
#include "PlayState.h"

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "PauseState.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"    
#include "OgreBulletCollisionsRay.h"

template<> PlayState* Ogre::Singleton<PlayState>::msSingleton = 0;


PlayState::PlayState(){
  

}

PlayState::~PlayState(){}

void
PlayState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _sceneMgr -> setAmbientLight(Ogre::ColourValue(1,1,1));
  _camera = _sceneMgr->getCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(5,20,20));
  _camera->lookAt(Ogre::Vector3(0,0,0));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  //_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));

  _numEntities = 0;    // Numero de Shapes instanciadas
  _timeLastObject = 0; // Tiempo desde que se añadio el ultimo objeto

  _mouse = InputManager::getSingletonPtr()->getMouse();
  
  // Creacion del modulo de debug visual de Bullet ------------------
  _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  _debugDrawer->setDrawWireframe(true);  
  SceneNode *node = _sceneMgr->getRootSceneNode()->
    createChildSceneNode("debugNode", Vector3::ZERO);
  node->attachObject(static_cast <SimpleRenderable *>(_debugDrawer));
  // Creacion del mundo (definicion de los limites y la gravedad) ---
  AxisAlignedBox worldBounds = AxisAlignedBox (
    Vector3 (-10000, -10000, -10000), 
    Vector3 (10000,  10000,  10000));
  Vector3 gravity = Vector3(0, -9.8, 0);

  _world = new OgreBulletDynamics::DynamicsWorld(_sceneMgr,
     worldBounds, gravity);
  _world->setDebugDrawer (_debugDrawer);
  _world->setShowDebugShapes (false);  // Muestra los collision shapes

  // Creacion de los elementos iniciales del mundo
  
  CreateInitialWorld();

  _exitGame = false;
}

void
PlayState::exit ()
{
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause()
{
}

void
PlayState::resume()
{
  // Se restaura el background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 0.0, 1.0));
}

bool
PlayState::frameStarted
(const Ogre::FrameEvent& evt)
{

  Ogre::Vector3 vt(0,0,0);     Ogre::Real tSpeed = 20.0;  
  _deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / _deltaT;
  bool mbleft, mbmiddle, mbright; // Botones del raton pulsados

  _world->stepSimulation(_deltaT); // Actualizar simulacion Bullet
  _timeLastObject -= _deltaT;

  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.

  //std::cout << posx << " " << posy <<"\n";
  mbmiddle = _mouse->getMouseState().buttonDown(OIS::MB_Middle);

  _camera->moveRelative(vt * _deltaT * tSpeed);
  if (_camera->getPosition().length() < 10.0) {
    _camera->moveRelative(-vt * _deltaT * tSpeed);
  }

  if(mbmiddle){
    float rotx = _mouse->getMouseState().X.rel * _deltaT * -1;
    float roty = _mouse->getMouseState().Y.rel * _deltaT * -1;
    _camera->yaw(Ogre::Radian(rotx));
    _camera->pitch(Ogre::Radian(roty));  
  }
  


  return true;
}

bool
PlayState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  _deltaT = evt.timeSinceLastFrame;
  _world->stepSimulation(_deltaT); // Actualizar simulacion Bullet

  return true;
}

void
PlayState::keyPressed
(const OIS::KeyEvent &e)
{
  // Tecla p --> PauseState.
  if (e.key == OIS::KC_P) {
    pushState(PauseState::getSingletonPtr());
  }
  if ((e.key == OIS::KC_B) && (_timeLastObject <= 0)) 
    AddDynamicObject(box);
  if ((e.key == OIS::KC_S) && (_timeLastObject <= 0)) 
    AddDynamicObject(sheep);
  if (e.key == OIS::KC_D) _world->setShowDebugShapes (true); 
  if (e.key == OIS::KC_H) _world->setShowDebugShapes (false); 
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);
}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertMouseButton(id));

    float F;
    RigidBody* body; Vector3 p; Ray r;
    int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
    int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.
    float x = posx/float(_viewport->getActualWidth());   // Pos x normalizada
    float y = posy/float(_viewport->getActualWidth());  // Pos y normalizada

   if(e.state.buttonDown(OIS::MB_Left)){
      std::cout << "BOTON IZQUIERDO PULSADO\n";
      F = 10;

      body = pickBody (p, r, x, y);

      if (body) {  
        if (!body->isStaticObject()) { 
          body->enableActiveState ();
          Vector3 relPos(p - body->getCenterOfMassPosition());
          Vector3 impulse (r.getDirection ());
          body->applyImpulse (impulse * F, relPos); 
        }
      }  
   }

   else if(e.state.buttonDown(OIS::MB_Right)){
      std::cout << "BOTON DERECHO PULSADO\n";
      F = 100; 

      body = pickBody (p, r, x, y);

      if (body) {  
        if (!body->isStaticObject()) { 
          body->enableActiveState ();
          Vector3 relPos(p - body->getCenterOfMassPosition());
          Vector3 impulse (r.getDirection ());
          body->applyImpulse (impulse * F, relPos); 
        }
      }  
   }

   else if(e.state.buttonDown(OIS::MB_Middle)){
      std::cout << "BOTON RUEDA PULSADO\n";
   }


}

void
PlayState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertMouseButton(id));
   /*if(e.state.buttonUp(OIS::MB_Left)){
      std::cout << "BOTON IZQUIERDO SOLTADO\n";
   }
   else if(e.state.buttonUp(OIS::MB_Right)){
      std::cout << "BOTON DERECHO SOLTADO\n";
   }
   else if(e.state.buttonUp(OIS::MB_Middle)){
      std::cout << "BOTON RUEDA SOLTADO\n";
   }*/
}

PlayState*
PlayState::getSingletonPtr ()
{
return msSingleton;
}

PlayState&
PlayState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

CEGUI::MouseButton PlayState::convertMouseButton(OIS::MouseButtonID id)
{
  CEGUI::MouseButton ceguiId;
  switch(id)
    {
    case OIS::MB_Left:
      ceguiId = CEGUI::LeftButton;
      break;
    case OIS::MB_Right:
      ceguiId = CEGUI::RightButton;
      break;
    case OIS::MB_Middle:
      ceguiId = CEGUI::MiddleButton;
      break;
    default:
      ceguiId = CEGUI::LeftButton;
    }
  return ceguiId;
}

void PlayState::CreateInitialWorld() {
  // Creacion de la entidad y del SceneNode ------------------------
  Plane plane1(Vector3(0,1,0), 0);    // Normal y distancia
  MeshManager::getSingleton().createPlane("p1",
  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
  200, 200, 1, 1, true, 1, 20, 20, Vector3::UNIT_Z);
  SceneNode* node = _sceneMgr->createSceneNode("ground");
  Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "p1");
  groundEnt->setMaterialName("Ground");
  node->attachObject(groundEnt);
  _sceneMgr->getRootSceneNode()->addChild(node);

  // Creamos forma de colision para el plano ----------------------- 
  OgreBulletCollisions::CollisionShape *Shape;
  Shape = new OgreBulletCollisions::StaticPlaneCollisionShape
    (Ogre::Vector3(0,1,0), 0);   // Vector normal y distancia
  OgreBulletDynamics::RigidBody *rigidBodyPlane = new 
    OgreBulletDynamics::RigidBody("rigidBodyPlane", _world);
  
  // Creamos la forma estatica (forma, Restitucion, Friccion) ------
  rigidBodyPlane->setStaticShape(Shape, 0.1, 0.8); 
  std::cout << "Hola" << std::endl;
  
  // Anadimos los objetos Shape y RigidBody ------------------------
  _shapes.push_back(Shape);      
  _bodies.push_back(rigidBodyPlane);

}

void PlayState::AddDynamicObject(TEDynamicObject tObject) {
  _timeLastObject = 0.25;   // Segundos para anadir uno nuevo... 

  Vector3 size = Vector3::ZERO; 
  Vector3 position = (_camera->getDerivedPosition() 
     + _camera->getDerivedDirection().normalisedCopy() * 10);
 
  Entity *entity = NULL;
  switch (tObject) {
  case sheep:
     entity = _sceneMgr->createEntity("Sheep" + 
     //StringConverter::toString(_numEntities), "sheep.mesh");
     StringConverter::toString(_numEntities), "CerdoIni.mesh");
    break;
  case box: default: 
    entity = _sceneMgr->createEntity("Box" + 
    //StringConverter::toString(_numEntities), "cube.mesh");
    StringConverter::toString(_numEntities), "CerdoIni.mesh");  
    entity->setMaterialName("cube");
  }

  SceneNode *node = _sceneMgr->getRootSceneNode()->
    createChildSceneNode();
  node->attachObject(entity);

  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = NULL; 
  OgreBulletCollisions::CollisionShape *bodyShape = NULL;
  OgreBulletDynamics::RigidBody *rigidBody = NULL;

  switch (tObject) {
  case sheep: 
    trimeshConverter = new 
      OgreBulletCollisions::StaticMeshToShapeConverter(entity);
    bodyShape = trimeshConverter->createConvex();
    delete trimeshConverter;
    break;
  case box: default: 
    AxisAlignedBox boundingB = entity->getBoundingBox();
    size = boundingB.getSize(); 
    size /= 2.0f;   // El tamano en Bullet se indica desde el centro
    bodyShape = new OgreBulletCollisions::BoxCollisionShape(size);
  }

  rigidBody = new OgreBulletDynamics::RigidBody("rigidBody" + 
     StringConverter::toString(_numEntities), _world);

  rigidBody->setShape(node, bodyShape,
         0.6 /* Restitucion */, 0.6 /* Friccion */,
         5.0 /* Masa */, position /* Posicion inicial */,
         Quaternion::IDENTITY /* Orientacion */);

  rigidBody->setLinearVelocity(
     _camera->getDerivedDirection().normalisedCopy() * 7.0); 

  _numEntities++;

  // Anadimos los objetos a las deques
  _shapes.push_back(bodyShape);   _bodies.push_back(rigidBody);
}

RigidBody* PlayState::pickBody (Vector3 &p, Ray &r, float x, float y) {
  r = _camera->getCameraToViewportRay (x, y);
  CollisionClosestRayResultCallback cQuery = 
    CollisionClosestRayResultCallback (r, _world, 10000);
  _world->launchRay(cQuery);
  if (cQuery.doesCollide()) {
    RigidBody* body = (RigidBody *) (cQuery.getCollidedObject());
    p = cQuery.getCollisionPoint();
    return body;
  }
  return NULL;
}