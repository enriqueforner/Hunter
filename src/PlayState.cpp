
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
#include <sstream>

#define THROW_FORCE 50.0
#define MAX_FORCE 75.0
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
  std::cout << "IN ENTER" << std::endl;
  _root = Ogre::Root::getSingletonPtr();
  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _sceneMgr -> setAmbientLight(Ogre::ColourValue(1,1,1));
  _camera = _sceneMgr->getCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(55,17,0));
  //_camera->setPosition(Ogre::Vector3(-45,17,0));
  _camera->lookAt(Ogre::Vector3(0,0,0));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);
  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(36.0f, 36.0f, 126.0f, 0.0f));
  double width = _viewport->getActualWidth();
  double height = _viewport->getActualHeight();
  _camera->setAspectRatio(width / height);
  //_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));

  _numEntities = 0;    // Numero de Shapes instanciadas
  _timeLastObject = 0; // Tiempo desde que se añadio el ultimo objeto

  _mouse = InputManager::getSingletonPtr()->getMouse();
  std::cout << "SceneManager Mouse _viewport" << std::endl;
  // Creacion del modulo de debug visual de Bullet ------------------
  _debugDrawer = new OgreBulletCollisions::DebugDrawer();
  _debugDrawer->setDrawWireframe(true);  
  SceneNode *node = _sceneMgr->getRootSceneNode()->
    createChildSceneNode("debugNode", Vector3::ZERO);
  node->attachObject(static_cast <SimpleRenderable *>(_debugDrawer));
  // Creacion del mundo (definicion de los limites y la gravedad) ---
  AxisAlignedBox worldBounds = AxisAlignedBox (
    Vector3 (-1000, -1000, -1000), 
    Vector3 (1000,  1000,  1000));
  Vector3 gravity = Vector3(0, -9.8, 0);

  _world = new OgreBulletDynamics::DynamicsWorld(_sceneMgr,
     worldBounds, gravity);
  _world->setDebugDrawer (_debugDrawer);
  _world->setShowDebugShapes (false);  // Muestra los collision shapes

  //CAMARA 2
  _aerialCamera = _sceneMgr->createCamera("AerialCamera");
  _aerialCamera->setPosition(Ogre::Vector3(30, 50, 0));
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
  std::vector<OBEntity*> *obentitiesPTR = new std::vector<OBEntity*>;
  _obEntities = *obentitiesPTR;
  
  CreateInitialWorld();
  _sceneMgr->setSkyBox(true, "MySky");
  _sPF = new ScenePlayFinal(_sceneMgr);
  _sPF -> PointsAndPower();
  _keyDownTime = 0.0;
  _shootKeyDown = false;
  _mouseRotation = Vector2::ZERO;
  _firstCol = true;
  _latestNodeCol = "none"; 
  _finalGame = false;
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().hide();
  std::cout << "SKYBOX SCENAPLAY" << std::endl;
  //Crear el movementcontroller y el physicscontroller
  _points = 0;
  int *pointsPtr;
  pointsPtr = &_points;
  std::cout << *pointsPtr << std::endl;
  _movementController = new MovementController(_sceneMgr,&_obEntities);
  _physicsController = new PhysicsController(_sceneMgr, _world, &_obEntities, pointsPtr);
  _lanzaranimationPig = true;
  _forcePercent = 0;
  _vector_anims_pig = new std::vector<Ogre::AnimationState*>;
   std::cout << "FIN ENTER" << std::endl;
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
   std::cout << "frameStarted PLAY" << std::endl;

  Ogre::Vector3 vt(0,0,0);     Ogre::Real tSpeed = 20.0;  
  _deltaT = evt.timeSinceLastFrame;

  _world->stepSimulation(_deltaT); // Actualizar simulacion Bullet
  _timeLastObject -= _deltaT;

  _camera->moveRelative(vt * _deltaT * tSpeed);
  if (_camera->getPosition().length() < 10.0) {
    _camera->moveRelative(-vt * _deltaT * tSpeed);
  }

  _camera->yaw(Degree(CAM_ROTATION_SPEED * _deltaT * _mouseRotation.x)); //, Node::TS_PARENT
  _camera->pitch(Degree(CAM_ROTATION_SPEED * _deltaT * _mouseRotation.y)); //, Node::TS_LOCAL
  _mouseRotation = Vector2::ZERO;

  //CONTROLAR LA DIRECCION DE LA CAMARA DEL PROYECTIL
  _projectileCamera->lookAt(_camera->getDerivedDirection());
  
  if(_trackedBody){
    _projectileCamera->setPosition(_trackedBody->getCenterOfMassPosition());
    Ogre::Vector3 trackedBodyPosition = _trackedBody->getCenterOfMassPosition();
    Ogre::Vector3 projectileLookAt(trackedBodyPosition.x - _camera->getPosition().x, trackedBodyPosition.y - _camera->getPosition().y, trackedBodyPosition.z - _camera->getPosition().z);
    //_projectileCamera->lookAt(_camera->getDerivedDirection());
    _projectileCamera->lookAt(trackedBodyPosition + projectileLookAt);
  }
   std::cout << "CAMERAS" << std::endl;
  if(_shootKeyDown){
    _keyDownTime = _keyDownTime + _deltaT;
  }
 
  if(_keyDownTime * THROW_FORCE > 100){
    _forcePercent = 100;
  }  
  else{
    _forcePercent = _keyDownTime * THROW_FORCE;
  }

  //_points++;
  _sPF->updatePower(_forcePercent);
  _sPF->updatePoints(_points);
  //std::cout<<_sPF->getSheet()->getChild("PowerWindow")->getUpdateMode() <<std::endl;
  //_sPF->getSheet()->getChild("PowerWindow")->update(_deltaT);
  //CEGUI::System::getSingleton().injectTimePulse(_deltaT);
  //DetectCollisionPig();
   std::cout << "points power" << std::endl;
  _physicsController->detectCollision();  //Este es el bueno. Hay que cambiarlo para que compruebe colisiones sobre todo
   std::cout << "pisis" << std::endl;
  _movementController->moveAll();
  std::cout << "collision moveall" << std::endl;
  if(_finalGame){
      pushState(FinalState::getSingletonPtr());
  }
  lifeWolf();
 std::cout << "wolf" << std::endl;
  if (_lanzaranimationPig){
    for (int i = 0; i < 3; ++i){
      std::ostringstream os;
      os << "pigA" <<i; 
      Ogre::AnimationState* animStatePig = _sceneMgr->getEntity(os.str())-> getAnimationState("SaltoR");
      animStatePig->setTimePosition(0.0);
      animStatePig->setEnabled(true);
      animStatePig->setLoop(true);
      _vector_anims_pig -> push_back(animStatePig);
    }
    _lanzaranimationPig = false;
  }
  

  for (int i = 0; i < 3; ++i){
      Ogre::AnimationState* animStatePig = _vector_anims_pig->at(i);
      if (animStatePig != NULL){
        if (animStatePig->hasEnded()){
          animStatePig->setTimePosition(0.0);
          animStatePig->setEnabled(false);
        }else{
          animStatePig->addTime(_deltaT);
        }
      }
  }
   std::cout << "animation" << std::endl;
  //RecorreVectorTAOAnadirMovimientoConstante();
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
   std::cout << "frameEnded PLAY" << std::endl;
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
  if (e.key == OIS::KC_M) {
    pushState(FinalState::getSingletonPtr());
  }
  if ((e.key == OIS::KC_B) && (_timeLastObject <= 0)) 
    //AddDynamicObject(box);
  if (e.key == OIS::KC_E){
    //_shootKeyDown = true;
  } 
  else{
    _keyDownTime = 0.0;
  }
  if (e.key == OIS::KC_H){
    OBEntity *obAux = new OBEntity("none");
    std::cout << "OBEntities" << std::endl;
    for(std::vector<OBEntity *>::iterator it = _obEntities.begin(); it != _obEntities.end(); ++it) {
      obAux = *it;
      std::cout << obAux->getType() <<std::endl;
    }
  } 
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
   //double tForce = 0.0;
   if (e.key == OIS::KC_ESCAPE) {
     _exitGame = true;
   }
  // if (e.key == OIS::KC_E){
  //   if(_timeLastObject <= 0){
  //     tForce = THROW_FORCE*_keyDownTime;
  //     if(tForce > MAX_FORCE){
  //       tForce = MAX_FORCE;
  //     }
  //     //std::cout << _keyDownTime << std::endl;
  //     AddAndThrowDynamicObject("rock", tForce); //poner aqui el tipo de cosa que tirar
  //   }
  //   _shootKeyDown = false;
  //   _keyDownTime = 0.0;
  // }
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
   CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertMouseButton(id));

    float F;
    RigidBody* body; Vector3 p; Ray r;
    int posx = _mouse->getMouseState().X.abs;   // Posicion del puntero
    int posy = _mouse->getMouseState().Y.abs;   //  en pixeles.
    float x = posx/float(_viewport->getActualWidth());   // Pos x normalizada
    float y = posy/float(_viewport->getActualWidth());  // Pos y normalizada

   if(e.state.buttonDown(OIS::MB_Left)){
      _shootKeyDown = true;
   }

   else if(e.state.buttonDown(OIS::MB_Right)){
      std::cout << "BOTON DERECHO PULSADO\n";
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
  double tForce = 0.0;
  if(_timeLastObject <= 0){
    tForce = THROW_FORCE*_keyDownTime;
    if(tForce > MAX_FORCE){
      tForce = MAX_FORCE;
    }
    _forcePercent = (tForce / MAX_FORCE) * 100;
    _sPF->updatePower(_forcePercent);
    AddAndThrowDynamicObject("rock", tForce); //poner aqui el tipo de cosa que tirar
    _forcePercent = 0;
  }
  _shootKeyDown = false;
  _keyDownTime = 0.0;
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

  Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create( "GroundJ", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

  Ogre::TextureUnitState* tuisTexture = mat->getTechnique(0)->getPass(0)->createTextureUnitState("ground2.jpg");

  MeshManager::getSingleton().createPlane("p1",
  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
  200, 200, 1, 1, true, 1, 35, 35, Vector3::UNIT_Z);
  SceneNode* node = _sceneMgr->createSceneNode("groundJ");
  Entity* groundEnt = _sceneMgr->createEntity("planeEnt", "p1");
  groundEnt->setMaterialName("GroundJ");
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
  
  ColocarWolfAndRedilAndPig();
  CrearBosqueAndColina();
  CreationWolf();
  std::cout << "BOSQUE AND COLINA COLOCADAS" <<std::endl;
}

void PlayState::AddAndThrowDynamicObject(std::string type, double force) {
  _timeLastObject = SHOOT_COOLDOWN;   // Segundos para anadir uno nuevo

  Vector3 size = Vector3::ZERO; 
  Vector3 position = (_camera->getDerivedPosition() 
     + _camera->getDerivedDirection().normalisedCopy() * 10);
 
  Entity *entity = NULL;
  std::stringstream uniqueName;
  uniqueName <<type << _numEntities;
  OBEntity *obentity = new OBEntity(uniqueName.str());

  entity = _sceneMgr->createEntity(uniqueName.str(), "PiedraLanzar.mesh");

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

  uniqueName << "rig";
  rigidBody = new OgreBulletDynamics::RigidBody(uniqueName.str(), _world);

  rigidBody->setShape(node, bodyShape,
         0.0 /* Restitucion */, 1.0 /* Friccion */,
         50.0 /* Masa */, position /* Posicion inicial */,
         Quaternion::IDENTITY /* Orientacion */);

  rigidBody->setLinearVelocity(
     _camera->getDerivedDirection().normalisedCopy() * force);  

  obentity->setRigidBody(rigidBody);

  _numEntities++;

  // Anadimos los objetos a las deques  
  _trackedBody = rigidBody;
  _obEntities.push_back(obentity);
}

void PlayState::CrearBosqueAndColina(){
    int distH = 0;
    int distV = 0;
    int numArboles = 0;
    int numArbolesGlobal = 0;
    int mover = 40;
    //Parte Izquierda y Parte Derecha
    for (int i = 0; i < 192; i++){
      std::stringstream uniqueName;
      uniqueName <<"tree" << numArbolesGlobal;
      Entity *entityArbol = _sceneMgr->createEntity(uniqueName.str(),"ArbolJuego.mesh");
      SceneNode *nodeArbol = _sceneMgr->createSceneNode(uniqueName.str());
      nodeArbol->attachObject(entityArbol);
      _sceneMgr->getRootSceneNode()->addChild(nodeArbol);

      OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterR = new 
        OgreBulletCollisions::StaticMeshToShapeConverter(entityArbol);

      OgreBulletCollisions::TriangleMeshCollisionShape *TrimeshR = 
        trimeshConverterR->createTrimesh(); 

      OgreBulletDynamics::RigidBody *rigidObjectA = new 
      OgreBulletDynamics::RigidBody(uniqueName.str(), _world);
        rigidObjectA->setShape(nodeArbol, TrimeshR, 0.5, 0.5, 0, Ogre::Vector3(-(0 + distV), 0, mover + distH), 
        Quaternion::IDENTITY); 
      distH = distH + 8;
      numArbolesGlobal = numArbolesGlobal + 1;
      numArboles = numArboles +1;
      if(numArboles == 8){
          distV = distV + 6;
          numArboles = 0;
          distH = 0;
      }
      if(numArbolesGlobal == 96){
          mover = -100;
          distV = 0;
      }
    }
    //Parte Final
    distV = 0;
    distH = 0;
    for (int i = 0; i < 30; ++i){
      std::stringstream uniqueName;
      uniqueName <<"tree" << numArbolesGlobal;
      Entity *entityArbol = _sceneMgr->createEntity(uniqueName.str(),"ArbolJuego.mesh");
      SceneNode *nodeArbol = _sceneMgr->createSceneNode(uniqueName.str());
      nodeArbol->attachObject(entityArbol);
      _sceneMgr->getRootSceneNode()->addChild(nodeArbol);

      OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterR = new 
        OgreBulletCollisions::StaticMeshToShapeConverter(entityArbol);

      OgreBulletCollisions::TriangleMeshCollisionShape *TrimeshR = 
        trimeshConverterR->createTrimesh(); 

      OgreBulletDynamics::RigidBody *rigidObjectA = new 
      OgreBulletDynamics::RigidBody(uniqueName.str(), _world);
        rigidObjectA->setShape(nodeArbol, TrimeshR, 0.5, 0.5, 0, Ogre::Vector3(-85 -(distV), 0, -47 + distH), 
        Quaternion::IDENTITY); 
      numArbolesGlobal = numArbolesGlobal + 1;
      distH = distH + 7;
      if(i==14){
          distV = distV + 6;
          distH = 3;
      }
    }

    Entity *entityColina = _sceneMgr->createEntity("Colina","ColinaJugador.mesh");
    SceneNode *nodeColina = _sceneMgr->createSceneNode("Colina");
    nodeColina->attachObject(entityColina);
    _sceneMgr->getRootSceneNode()->addChild(nodeColina);

    OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverterR = new 
      OgreBulletCollisions::StaticMeshToShapeConverter(entityColina);

    OgreBulletCollisions::TriangleMeshCollisionShape *TrimeshR = 
      trimeshConverterR->createTrimesh(); 

    OgreBulletDynamics::RigidBody *rigidObjectColina = new 
    OgreBulletDynamics::RigidBody("Colina", _world);
      rigidObjectColina->setShape(nodeColina, TrimeshR, 0.5, 0.5, 0, Ogre::Vector3(55,0.50,0), 
      Quaternion::IDENTITY); 
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
  rigidObjectR->setShape(nodeRedil, TrimeshR, 0.5, 0.5, 0, Ogre::Vector3(30,0,0), 
       Quaternion::IDENTITY);        
  
  int posx[] = {30,35,35};
  int posz[] = {0,-2,2};
  int posD[] = {0,-120,200};
  
  for (int i = 0; i < 3; ++i){
      std::ostringstream os;
      os << "pigA" << i;

      Ogre::Entity* entc = _sceneMgr->createEntity(os.str(), "CerdoAnim.mesh");
      Ogre::SceneNode* nodecer = _sceneMgr->createSceneNode(os.str());
      nodecer->attachObject(entc);
      nodecer-> setPosition(posx[i],0,posz[i]);
      // roll --> z
      // pitch --> x
      // yaw --> y
      nodecer->yaw(Ogre::Degree(posD[i]));
      _sceneMgr->getRootSceneNode()->addChild(nodecer);
      
  }
}

 void PlayState::CreationWolf(){ //Poner wolves
    int posH = 4;
    int H = -20;
    int V = 0;
    for (int i = 0; i < 19; ++i){  
      Entity *entity = NULL;
      
      std::stringstream uniqueName;
      uniqueName <<"wolf" << _numEntities; 
      
      OBEntity *obentity = new OBEntity(uniqueName.str()); 
      
      entity = _sceneMgr->createEntity(uniqueName.str(), "Lobo.mesh");     

      SceneNode *node = _sceneMgr->getRootSceneNode()->
      createChildSceneNode(entity->getName());
      node->attachObject(entity);

      node->yaw(Ogre::Degree(180));

      obentity->setSceneNode(node);

      OgreBulletCollisions::StaticMeshToShapeConverter *trimeshConverter = NULL; 
      OgreBulletCollisions::CollisionShape *bodyShape = NULL;
      OgreBulletDynamics::RigidBody *rigidBody = NULL;

      trimeshConverter = new 
      OgreBulletCollisions::StaticMeshToShapeConverter(entity);
      bodyShape = trimeshConverter->createConvex();
      delete trimeshConverter;

      obentity->setCollisionShape(bodyShape);
      rigidBody = new OgreBulletDynamics::RigidBody(uniqueName.str(), _world);

      rigidBody->setShape(node, bodyShape,
         0.0 /* Restitucion */, 0.6 /* Friccion */,
         150.0 /* Masa */, Ogre::Vector3(-70 + V, 0, H + posH),  // 0,0,35
         node->_getDerivedOrientation()/* Orientacion */); 
      rigidBody->setLinearVelocity(Ogre::Vector3::ZERO);

      obentity->setRigidBody(rigidBody);  
   
      _obEntities.push_back(obentity);
      _numEntities ++;
      posH = posH + 4;
      if(i ==9){
          H = -20;
          posH = 6;
          V = -5;
      }
    }  
 }

 void PlayState::isFinalGame(){
    _finalGame = true;
 }

 void PlayState::lifeWolf(){
    OBEntity *obAux = new OBEntity("name");
    int deadWolf = 0;
    int i = 0;
    for(std::vector<OBEntity *>::iterator it = _obEntities.begin(); it != _obEntities.end(); ++it) {
      obAux = *it;
      std::ostringstream os;
      os << "wolf" <<i;
      if (obAux->getType().compare(os.str())==0){
          if (obAux->getHealth()==0){ 
              deadWolf = deadWolf +1;
          }
      }
      i++;
    }
    if(deadWolf == 19){
        isFinalGame();
    }
 }