/*********************************************************************
 * Módulo 2. Curso de Experto en Desarrollo de Videojuegos
 * Autor: Carlos González Morcillo     Carlos.Gonzalez@uclm.es
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/
#include "MyFrameListener.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"		
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "OgreBulletCollisionsRay.h"

MyFrameListener::MyFrameListener(RenderWindow* win, 
				 Camera* cam, 
				 OverlayManager *om, 
				 SceneManager *sm) {
  OIS::ParamList param;
  size_t windowHandle;  std::ostringstream wHandleStr;

  _camera = cam;  _overlayManager = om; _sceneManager = sm;  _win = win;
  
  win->getCustomAttribute("WINDOW", &windowHandle);
  wHandleStr << windowHandle;
  param.insert(std::make_pair("WINDOW", wHandleStr.str()));
  
  _inputManager = OIS::InputManager::createInputSystem(param);
  _keyboard = static_cast<OIS::Keyboard*>
    (_inputManager->createInputObject(OIS::OISKeyboard, false));
  _mouse = static_cast<OIS::Mouse*>
    (_inputManager->createInputObject(OIS::OISMouse, false));
  _mouse->getMouseState().width = win->getWidth();
  _mouse->getMouseState().height = win->getHeight();

  _numEntities = 0;    // Numero de Shapes instanciadas
  _timeLastObject = 0; // Tiempo desde que se añadio el ultimo objeto

  // Creacion del modulo de debug visual de Bullet ------------------
  _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  _debugDrawer->setDrawWireframe(true);	 
  SceneNode *node = _sceneManager->getRootSceneNode()->
    createChildSceneNode("debugNode", Vector3::ZERO);
  node->attachObject(static_cast <SimpleRenderable *>(_debugDrawer));

  // Creacion del mundo (definicion de los limites y la gravedad) ---
  AxisAlignedBox worldBounds = AxisAlignedBox (
    Vector3 (-10000, -10000, -10000), 
    Vector3 (10000,  10000,  10000));
  Vector3 gravity = Vector3(0, -9.8, 0);

  _world = new OgreBulletDynamics::DynamicsWorld(_sceneManager,
 	   worldBounds, gravity);
  _world->setDebugDrawer (_debugDrawer);
  _world->setShowDebugShapes (false);  // Muestra los collision shapes

  // Creacion de los elementos iniciales del mundo
  CreateInitialWorld();
}

MyFrameListener::~MyFrameListener() {
  _inputManager->destroyInputObject(_keyboard);
  _inputManager->destroyInputObject(_mouse);
  OIS::InputManager::destroyInputSystem(_inputManager);

  // Eliminar cuerpos rigidos --------------------------------------
  std::deque <OgreBulletDynamics::RigidBody *>::iterator 
     itBody = _bodies.begin();
  while (_bodies.end() != itBody) {   
    delete *itBody;  ++itBody;
  } 
 
  // Eliminar formas de colision -----------------------------------
  std::deque<OgreBulletCollisions::CollisionShape *>::iterator 
    itShape = _shapes.begin();
  while (_shapes.end() != itShape) {   
    delete *itShape; ++itShape;
  } 

  _bodies.clear();  _shapes.clear();

  // Eliminar mundo dinamico y debugDrawer -------------------------
  delete _world->getDebugDrawer();    _world->setDebugDrawer(0);
  delete _world;
}

void MyFrameListener::CreateInitialWorld() {
  // Creacion de la entidad y del SceneNode ------------------------
  Plane plane1(Vector3(0,1,0), 0);    // Normal y distancia
  MeshManager::getSingleton().createPlane("p1",
	ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
	200, 200, 1, 1, true, 1, 20, 20, Vector3::UNIT_Z);
  SceneNode* node = _sceneManager->createSceneNode("ground");
  Entity* groundEnt = _sceneManager->createEntity("planeEnt", "p1");
  groundEnt->setMaterialName("Ground");
  node->attachObject(groundEnt);
  _sceneManager->getRootSceneNode()->addChild(node);

  // Creamos forma de colision para el plano ----------------------- 
  OgreBulletCollisions::CollisionShape *Shape;
  Shape = new OgreBulletCollisions::StaticPlaneCollisionShape
    (Ogre::Vector3(0,1,0), 0);   // Vector normal y distancia
  OgreBulletDynamics::RigidBody *rigidBodyPlane = new 
    OgreBulletDynamics::RigidBody("rigidBodyPlane", _world);

  // Creamos la forma estatica (forma, Restitucion, Friccion) ------
  rigidBodyPlane->setStaticShape(Shape, 0.1, 0.8); 
  
  // Anadimos los objetos Shape y RigidBody ------------------------
  _shapes.push_back(Shape);      _bodies.push_back(rigidBodyPlane);
}

void MyFrameListener::AddDynamicObject(TEDynamicObject tObject) {
  _timeLastObject = 0.25;   // Segundos para anadir uno nuevo... 

  Vector3 size = Vector3::ZERO;	
  Vector3 position = (_camera->getDerivedPosition() 
     + _camera->getDerivedDirection().normalisedCopy() * 10);
 
  Entity *entity = NULL;
  switch (tObject) {
  case sheep:
     entity = _sceneManager->createEntity("Sheep" + 
     StringConverter::toString(_numEntities), "sheep.mesh");
    break;
  case box: default: 
    entity = _sceneManager->createEntity("Box" + 
    StringConverter::toString(_numEntities), "cube.mesh");
    entity->setMaterialName("cube");
  }

  SceneNode *node = _sceneManager->getRootSceneNode()->
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

RigidBody* MyFrameListener::pickBody (Vector3 &p, Ray &r, float x, float y) {
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

bool MyFrameListener::frameStarted(const Ogre::FrameEvent& evt) {
  Ogre::Vector3 vt(0,0,0);     Ogre::Real tSpeed = 20.0;  
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  int fps = 1.0 / deltaT;
  bool mbleft, mbmiddle, mbright; // Botones del raton pulsados

  _world->stepSimulation(deltaT); // Actualizar simulacion Bullet
  _timeLastObject -= deltaT;

  _keyboard->capture();
  if (_keyboard->isKeyDown(OIS::KC_ESCAPE)) return false;
  if ((_keyboard->isKeyDown(OIS::KC_B)) && (_timeLastObject <= 0)) 
    AddDynamicObject(box);
  if ((_keyboard->isKeyDown(OIS::KC_S)) && (_timeLastObject <= 0)) 
    AddDynamicObject(sheep);
  if (_keyboard->isKeyDown(OIS::KC_D)) _world->setShowDebugShapes (true); 
  if (_keyboard->isKeyDown(OIS::KC_H)) _world->setShowDebugShapes (false); 

  int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
  int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.

  _camera->moveRelative(vt * deltaT * tSpeed);
  if (_camera->getPosition().length() < 10.0) {
    _camera->moveRelative(-vt * deltaT * tSpeed);
  }

  _mouse->capture();

 // Si usamos la rueda, desplazamos en Z la camara ------------------
  vt+= Ogre::Vector3(0,0,-10)*deltaT * _mouse->getMouseState().Z.rel;   
  _camera->moveRelative(vt * deltaT * tSpeed);

  // Botones del raton pulsados? -------------------------------------
  mbleft = _mouse->getMouseState().buttonDown(OIS::MB_Left);
  mbmiddle = _mouse->getMouseState().buttonDown(OIS::MB_Middle);
  mbright = _mouse->getMouseState().buttonDown(OIS::MB_Right);

  if (mbmiddle) { // Con boton medio pulsado, rotamos camara ---------
    float rotx = _mouse->getMouseState().X.rel * deltaT * -1;
    float roty = _mouse->getMouseState().Y.rel * deltaT * -1;
    _camera->yaw(Ogre::Radian(rotx));
    _camera->pitch(Ogre::Radian(roty));
  }

  if (mbleft || mbright) { // Con otros botones, aplicamos impulso -----
    float F = 10;  if (mbright) F = 100; 
    RigidBody* body; Vector3 p; Ray r;
    float x = posx/float(_win->getWidth());   // Pos x normalizada
    float y = posy/float(_win->getHeight());  // Pos y normalizada
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
  
  
  Ogre::OverlayElement *oe;
  oe = _overlayManager->getOverlayElement("cursor");
  oe->setLeft(posx);  oe->setTop(posy);

  oe = _overlayManager->getOverlayElement("fpsInfo");
  oe->setCaption(Ogre::StringConverter::toString(fps));

  oe = _overlayManager->getOverlayElement("nEntitiesInfo");
  oe->setCaption(Ogre::StringConverter::toString(_numEntities));


  return true;
}

bool MyFrameListener::frameEnded(const Ogre::FrameEvent& evt) {
  Real deltaT = evt.timeSinceLastFrame;
  _world->stepSimulation(deltaT); // Actualizar simulacion Bullet
  return true;
}
