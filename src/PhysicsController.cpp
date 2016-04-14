
#include "PhysicsController.h"

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"   
#include "OgreBulletCollisionsRay.h"

#include <iostream>
#include <vector>

#define THROW_FORCE 50.0
#define T_SPEED 20.0
#define CAM_ROTATION_SPEED 20.0
#define SHOOT_COOLDOWN 1

//MUDAR LO DE LAS COLISIONES A ESTA CLASE
PhysicsController::PhysicsController(){}

PhysicsController::PhysicsController(Ogre::SceneManager *sceneMgr, OgreBulletDynamics::DynamicsWorld *world){
	_sceneMgr = sceneMgr;
	_world = world;
}

//~PhysicsController(){}

Ogre::SceneManager *PhysicsController::getSceneManager(){
	return _sceneMgr;
}
OgreBulletDynamics::DynamicsWorld *PhysicsController::getWorld(){
	return _world;
}
void PhysicsController::setSceneManager(Ogre::SceneManager *sceneMgr){
	_sceneMgr = sceneMgr;
}
void PhysicsController::setWorld(OgreBulletDynamics::DynamicsWorld *world){
	_world = world;
}


Ogre::Vector2 PhysicsController::detectCollision(){  //hay que ponerle todos los casos, colisiones de objetos de X tipo con X tipo y tal
  Ogre::Vector2 colliders (0,0);
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
        node = obOB_A->getRootNode(); /*delete obOB_A*/;
      }
      else if ((obOB_B != obDrain) && (obOB_B)) {
        node = obOB_B->getRootNode(); /*delete obOB_B*/;
      }
      if (node) {
        std::cout << node->getName() << "ESTA TOCANDO" << std::endl; 
      }
    }
    //} 
  }
  return colliders;
}

//std::vector <OBEntity> _obEntities; //Enemigos, proyectiles, escenario...






