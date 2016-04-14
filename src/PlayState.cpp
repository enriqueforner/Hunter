
#include "PlayState.h"

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "PauseState.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"    
#include "OgreBulletCollisionsRay.h"

#include <iostream>
#include <vector>

#define THROW_FORCE 50.0
#define MAX_FORCE 200.0
#define T_SPEED 20.0
#define CAM_ROTATION_SPEED 20.0
#define SHOOT_COOLDOWN 1

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
  _world->setShowDebugShapes (true);  // Muestra los collision shapes

  //CAMARA 2
  _aerialCamera = _sceneMgr->createCamera("AerialCamera");
  _aerialCamera->setPosition(Ogre::Vector3(0, 70, -50));
  _aerialCamera->lookAt(Ogre::Vector3(0, -1, 0));
  _aerialCamera->setNearClipDistance(5);
  
  //CAMARA 3
  _projectileCamera = _sceneMgr->createCamera("ProjectileCamera");
  _projectileCamera->setPosition(Ogre::Vector3(0, 0, 0));
  //_projectileCamera->lookAt(Ogre::Vector3(0, 0, 0));
  //CONTROLAR LA DIRECCION DE LA PROJECTILECAMERA
  _projectileCamera->lookAt(_camera->getDerivedDirection());
  _projectileCamera->setNearClipDistance(5);

  _trackedBody = NULL;

  // Creacion de los elementos iniciales del mundo
  
  CreateInitialWorld();

  _keyDownTime = 0.0;
  _shootKeyDown = false;
  _mouseRotation = Vector2::ZERO;

  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();

  _exitGame = false;
}

void
PlayState::exit ()
{
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
PlayState::pause()
{
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().show();
}

void
PlayState::resume()
{
  // Se restaura el background colour.
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
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

  _camera->yaw(Degree(CAM_ROTATION_SPEED * _deltaT * _mouseRotation.x)); //, Node::TS_PARENT
  _camera->pitch(Degree(CAM_ROTATION_SPEED * _deltaT * _mouseRotation.y)); //, Node::TS_LOCAL
  _mouseRotation = Vector2::ZERO;

  //CONTROLAR LA DIRECCION DE LA CAMARA DEL PROYECTIL
  _projectileCamera->lookAt(_camera->getDerivedDirection());

  if(mbmiddle){
    float rotx = _mouse->getMouseState().X.rel * _deltaT * -1;
    float roty = _mouse->getMouseState().Y.rel * _deltaT * -1;
    _camera->yaw(Ogre::Radian(rotx));
    _camera->pitch(Ogre::Radian(roty));  
  }
  
  if(_trackedBody){
    _projectileCamera->setPosition(_trackedBody->getCenterOfMassPosition());
  }

  if(_shootKeyDown){
    _keyDownTime = _keyDownTime + _deltaT;
  }
  DetectCollisionPig();
  
  RecorreVectorTAOAnadirMovimientoConstante();
  //std::cout << "Hasta aqui todo bien 1" << std::endl;

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
  if (e.key == OIS::KC_E){
    _shootKeyDown = true;
  } 
  else{
    _keyDownTime = 0.0;
  }
  if (e.key == OIS::KC_D) _world->setShowDebugShapes (true); 
  if (e.key == OIS::KC_H) _world->setShowDebugShapes (false); 
  if (e.key == OIS::KC_A){ //AerialCamera
    _root->getAutoCreatedWindow()->removeAllViewports();
    _viewport = _root->getAutoCreatedWindow()->addViewport(_aerialCamera);
    double width = _viewport->getActualWidth();
    double height = _viewport->getActualHeight();
    _aerialCamera->setAspectRatio(width / height);
  }
  if (e.key == OIS::KC_D){ //DefaultCamera
    _root->getAutoCreatedWindow()->removeAllViewports();
    _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
    double width = _viewport->getActualWidth();
    double height = _viewport->getActualHeight();
    _camera->setAspectRatio(width / height);
  }

  if (e.key == OIS::KC_S && _trackedBody){
     _root->getAutoCreatedWindow()->removeAllViewports();
     _viewport = _root->getAutoCreatedWindow()->addViewport(_projectileCamera);
     double width = _viewport->getActualWidth();
     double height = _viewport->getActualHeight();
     _projectileCamera->setAspectRatio(width / height);
  } 
   
}

void
PlayState::keyReleased
(const OIS::KeyEvent &e)
{
  double tForce = 0.0;
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
  if (e.key == OIS::KC_E){
    if(_timeLastObject <= 0){
      tForce = THROW_FORCE*_keyDownTime;
      if(tForce > MAX_FORCE){
        tForce = MAX_FORCE;
      }
      AddAndThrowDynamicObject("type", tForce); //poner aqui el tipo de cosa que tirar
    }
    _shootKeyDown = false;
    _keyDownTime = 0.0;
  }
}

void
PlayState::mouseMoved
(const OIS::MouseEvent &e)
{  /* OIS Y CEGUI SINCRONIZADOS POR FIN */
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);
  CEGUI::Vector2f posCegui = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();
  OIS::MouseState &mutableMouseState = const_cast<OIS::MouseState &>(_mouse->getMouseState());
  mutableMouseState.X.abs = posCegui.d_x;
  mutableMouseState.Y.abs = posCegui.d_y;

  _mouseRotation.x = e.state.X.rel;
  _mouseRotation.y = e.state.Y.rel;

}

void
PlayState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  /*  */
  //PARECE QUE OIS Y CEGUI YA ESTAN SINCRONIZADOS PERO SIGUEN SIN IR BIEN LOS EMPUJONES (POR UN POQUILLO)
   std::cout << "OIS X: " << _mouse->getMouseState().X.abs << "\n"; 
   std::cout << "OIS Y: " << _mouse->getMouseState().Y.abs << "\n";

   CEGUI::Vector2f posCegui = CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().getPosition();

   std::cout << "CEGUI X: " << posCegui.d_x << "\n"; 
   std::cout << "CEGUI Y: " << posCegui.d_y << "\n";


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

  ColocarWolfAndRedilAndPig();
  TEDynamicObjectMovement();
  //CREACION DEL SUPER CERDO CHOCADOR
  Entity *entity2 = _sceneMgr->createEntity("CERDO","CerdoIni.mesh");
  SceneNode *node2 = _sceneMgr->createSceneNode("CERDO");
  node2->attachObject(entity2);
  //node2-> setPosition(9,7,20);
  //node2->setScale(3,3,3);
  
  _sceneMgr->getRootSceneNode()->addChild(node2);
  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter2 = new 
    OgreBulletCollisions::StaticMeshToShapeConverter(entity2);

  OgreBulletCollisions::TriangleMeshCollisionShape *Trimesh2 = 
    trimeshConverter2->createTrimesh();

  OgreBulletDynamics::RigidBody *rigidObject2 = new 
    OgreBulletDynamics::RigidBody("CERDO", _world);
  rigidObject2->setShape(node2, Trimesh2, 3, 3, 0, Ogre::Vector3(1,4,2), 
       Quaternion::IDENTITY);


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
void PlayState::AddAndThrowDynamicObject(std::string type, double force) {
  //AddDynamicObject(tObject);
  _timeLastObject = SHOOT_COOLDOWN;   // Segundos para anadir uno nuevo... 

  Vector3 size = Vector3::ZERO; 
  Vector3 position = (_camera->getDerivedPosition() 
     + _camera->getDerivedDirection().normalisedCopy() * 10);
 
  Entity *entity = NULL;
  //OBEntity *obentity = new OBEntity(type);
  OBEntity *obentity = new OBEntity("type");

  entity = _sceneMgr->createEntity("OBEntity" + 
  //StringConverter::toString(_numEntities), "sheep.mesh");
  StringConverter::toString(_numEntities), "CerdoIni.mesh");

  SceneNode *node = _sceneMgr->getRootSceneNode()->
    createChildSceneNode(entity->getName());
  node->attachObject(entity);

  obentity->setSceneNode(node);

  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = NULL; 
  OgreBulletCollisions::CollisionShape *bodyShape = NULL;
  OgreBulletDynamics::RigidBody *rigidBody = NULL;


  trimeshConverter = new 
    OgreBulletCollisions::StaticMeshToShapeConverter(entity);
  bodyShape = trimeshConverter->createConvex();
  delete trimeshConverter;

  obentity->setCollisionShape(bodyShape);

  rigidBody = new OgreBulletDynamics::RigidBody("rigidBody" + 
     StringConverter::toString(_numEntities), _world);

  rigidBody->setShape(node, bodyShape,
         0.6 /* Restitucion */, 0.6 /* Friccion */,
         5.0 /* Masa */, position /* Posicion inicial */,
         Quaternion::IDENTITY /* Orientacion */);

  rigidBody->setLinearVelocity(
     _camera->getDerivedDirection().normalisedCopy() * force);  //7.0

  obentity->setRigidBody(rigidBody);

  _numEntities++;

  // Anadimos los objetos a las deques
  _shapes.push_back(bodyShape);   _bodies.push_back(rigidBody);
  _trackedBody = rigidBody;
  _obEntities.push_back(obentity);
}

//SOLO DETECTA LA COLISION CON EL CERDO QUE SE ESTA MOVIENDO
void PlayState::DetectCollisionPig() {
  //CODIGO COMENTADO PARA DETECTAR COLISIONES EN LOS DEMAS OBJETOS YA SEA EN MOVIMENTO O QUIETOS
  btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
  // DE MOMENTO DETECTA COLISIONES SI PASA MUY CERCA
  int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();

  for (int i=0;i<numManifolds;i++) {
    btPersistentManifold* contactManifold = 
      bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
    btCollisionObject* obA = 
      (btCollisionObject*)(contactManifold->getBody0());
    btCollisionObject* obB = 
      (btCollisionObject*)(contactManifold->getBody1());
    //OgreBulletCollisions::Object *obDrain = _world->findObject(drain);
    //CHOCAR CON LOBOS
    //for (int i = 0; i < 3; ++i){
        
    //std::ostringstream os;
    //os << "CerdoMaloE" << i;
    Ogre::SceneNode* drain = _sceneMgr->getSceneNode("CerdoMaloE");

    OgreBulletCollisions::Object *obDrain = _world->findObject(drain);  
    
    OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
    OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

    if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
      Ogre::SceneNode* node = NULL;
      if ((obOB_A != obDrain) && (obOB_A)) {
        node = obOB_A->getRootNode(); /*delete obOB_A;*/
      }
      else if ((obOB_B != obDrain) && (obOB_B)) {
        node = obOB_B->getRootNode(); /*delete obOB_B;*/
      }
      if (node) {
        std::cout << node->getName() << "ESTA TOCANDO" << std::endl; 
        //delete node;

      }
    }
    //} 
  }
}

void PlayState::ColocarWolfAndRedilAndPig() {
  // CREACION REDIL PARA PONER ANIMALES
  Entity *entityRedil = _sceneMgr->createEntity("Redil","Redil.mesh");
  SceneNode *nodeRedil = _sceneMgr->createSceneNode("Redil");
  nodeRedil->attachObject(entityRedil);
  //nodeRedil-> setPosition(9,7,20);

  _sceneMgr->getRootSceneNode()->addChild(nodeRedil);
  OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterR = new 
          OgreBulletCollisions::StaticMeshToShapeConverter(entityRedil);

  OgreBulletCollisions::TriangleMeshCollisionShape *TrimeshR = 
          trimeshConverterR->createTrimesh();

  OgreBulletDynamics::RigidBody *rigidObjectR = new 
    OgreBulletDynamics::RigidBody("Redil", _world);
  rigidObjectR->setShape(nodeRedil, TrimeshR, 0.5, 0.5, 0, Vector3::ZERO, 
       Quaternion::IDENTITY);        

  int posx[5] = {10,15,20,25,30};
  for (int i = 0; i < 5; ++i){
      std::ostringstream os;
      os << "Wolf" << i;

      Entity *entityWolf = _sceneMgr->createEntity(os.str(),"Lobo.mesh");
      SceneNode *nodeWolf = _sceneMgr->createSceneNode(os.str());
      nodeWolf->attachObject(entityWolf);
      nodeWolf-> setPosition(10,200,posx[i]);

      _sceneMgr->getRootSceneNode()->addChild(nodeWolf);
      OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterW = new 
          OgreBulletCollisions::StaticMeshToShapeConverter(entityWolf);

      OgreBulletCollisions::TriangleMeshCollisionShape *TrimeshW = 
          trimeshConverterW->createTrimesh();
      OgreBulletDynamics::RigidBody *rigidObjectW = new 
        OgreBulletDynamics::RigidBody(os.str(), _world);
      rigidObjectW->setShape(nodeWolf, TrimeshW, 0.5, 0.5, 0,  Ogre::Vector3(0, 0, posx[i]), 
        Quaternion::IDENTITY);        
      
  }
}

void PlayState::TEDynamicObjectMovement(){
    //CODIGO COMENTADO PARA AÑADIR 200
    //int posx[3] = {7,14,21};
    //for (int i = 0; i < 3; ++i){  
      TEDynamicObject taO;
      Entity *entity = NULL;
      //OBEntity *obentity = new OBEntity(taO);
      OBEntity *obentity = new OBEntity("type");

      entity = _sceneMgr->createEntity("CerdoMaloE" /*+ 
      //StringConverter::toString(_numEntities), "sheep.mesh");
      StringConverter::toString(i)*/ ,"CerdoIni.mesh");

      SceneNode *node = _sceneMgr->getRootSceneNode()->
      createChildSceneNode(entity->getName());
      node->attachObject(entity);

      obentity->setSceneNode(node);

      OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = NULL; 
      OgreBulletCollisions::CollisionShape *bodyShape = NULL;
      OgreBulletDynamics::RigidBody *rigidBody = NULL;

      trimeshConverter = new 
      OgreBulletCollisions::StaticMeshToShapeConverter(entity);
      bodyShape = trimeshConverter->createConvex();
      delete trimeshConverter;

      obentity->setCollisionShape(bodyShape);

      rigidBody = new OgreBulletDynamics::RigidBody("CerdoMaloE" /*+ 
      StringConverter::toString(i),*/, _world);

      rigidBody->setShape(node, bodyShape,
         0.6 /* Restitucion */, 0.6 /* Friccion */,
         5.0 /* Masa */, Ogre::Vector3(0, 0, 35),
         Quaternion::IDENTITY /* Orientacion */);
      rigidBody->setLinearVelocity(Ogre::Vector3(0,0,7));  
   
      _shapesC.push_back(bodyShape);   _bodiesC.push_back(rigidBody);
    //}
 } 

 void PlayState::RecorreVectorTAOAnadirMovimientoConstante(){
    for(std::vector<OgreBulletDynamics::RigidBody *>::iterator it = _bodiesC.begin(); it != _bodiesC.end(); ++it) {
          (*it) -> setLinearVelocity(Ogre::Vector3(0,0,1));  
    }
 }
 


//CODIGO DEL CHOQUE CON LOBOS

 /*
//OgreBulletCollisions::Object *obDrain = _world->findObject(drain);
    //CHOCAR CON LOBOS
    for (int i = 0; i < 5; ++i){
        std::ostringstream os;
        os << "Wolf" << i;
        Ogre::SceneNode* drain = _sceneMgr->getSceneNode(os.str());

        OgreBulletCollisions::Object *obDrain = _world->findObject(drain);  
    

        OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
        OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

    if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
      Ogre::SceneNode* node = NULL;
      if ((obOB_A != obDrain) && (obOB_A)) {
        node = obOB_A->getRootNode(); /*delete obOB_A*/;
    /*  }
      else if ((obOB_B != obDrain) && (obOB_B)) {
        node = obOB_B->getRootNode(); /*delete obOB_B*/;
      /*}
      if (node) {
        std::cout << node->getName() << "ESTA TOCANDO" << std::endl;
        
      }
    }
    } */