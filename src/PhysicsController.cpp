
#include "PhysicsController.h"

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"   
#include "OgreBulletCollisionsRay.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "PlayState.h"

#define THROW_FORCE 50.0
#define T_SPEED 20.0
#define CAM_ROTATION_SPEED 20.0
#define SHOOT_COOLDOWN 1

//MUDAR LO DE LAS COLISIONES A ESTA CLASE
PhysicsController::PhysicsController(){
	_firstCol = true;
}

PhysicsController::PhysicsController(Ogre::SceneManager *sceneMgr, OgreBulletDynamics::DynamicsWorld *world, std::vector <OBEntity*> *obEntities, int *pointsPtr){
	_sceneMgr = sceneMgr;
	_world = world;
	//_movementController = movementController;
	_obEntities = obEntities;
	_pointsPtr = pointsPtr;
	_firstCol = true;
	_rocks = new std::vector<String>;
	_finalGame = true;
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

void PhysicsController::detectCollision(){
  if (_finalGame){
  btCollisionWorld *bulletWorld = _world->getBulletCollisionWorld();
  int numManifolds = bulletWorld->getDispatcher()->getNumManifolds();
 
  for (int i=0;i<numManifolds;i++) {
    btPersistentManifold* contactManifold = 
      bulletWorld->getDispatcher()->getManifoldByIndexInternal(i);
    btCollisionObject* obA = 
      (btCollisionObject*)(contactManifold->getBody0());
    btCollisionObject* obB = 
      (btCollisionObject*)(contactManifold->getBody1());
    
    Ogre::SceneNode* nodeA = NULL;
    Ogre::SceneNode* nodeB = NULL;
    
    OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
    OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

    nodeA = obOB_A->getRootNode();
    nodeB = obOB_B->getRootNode();

    if(nodeA && nodeB){
    	if((nodeA->getName().find("wolf") == 0)||(nodeB->getName().find("wolf") == 0)){
    		Ogre::SceneNode* node = NULL;
    		bool goodCollision = true;
    		if(nodeA->getName().find("rock") == 0){
    			node = obOB_A->getRootNode();
    			for(std::vector<String>::iterator it = _rocks->begin(); it != _rocks->end(); ++it) {
    				if (node->getName().find(*it) == 0){
    					goodCollision = false;
    				}
    			}
    		}
    		else if(nodeB->getName().find("rock") == 0){
    			node = obOB_B->getRootNode();
    			for(std::vector<String>::iterator it = _rocks->begin(); it != _rocks->end(); ++it) {
    				if (node->getName().find(*it) == 0){
    					goodCollision = false;
    				}
    			}
    		}
    		else if(nodeB->getName().find("tree") == 0){
    			node = obOB_B->getRootNode();
    			goodCollision = false;
    		}
    		else if(nodeB->getName().find("Redil") == 0){
    			node = obOB_B->getRootNode();
    			_finalGame = false;
    			goodCollision = false;
    		}
    		else if(nodeB->getName().find("wolf") == 0){
    			node = obOB_B->getRootNode();
    			goodCollision = false;
    		}
    		if(goodCollision){
    			//POINTS
    			std::cout << "DETECTED COLLISION: " << node->getName() << std::endl; 
        		if (nodeB->getName().find("wolf") == 0){
        			std::cout << nodeB->getName() << std::endl;
        			int points = getOBEntityByName(nodeB->getName())->decreaseHealth();
        			*_pointsPtr = *_pointsPtr + points;
        			std::cout << points << std::endl;
        			std::cout << getOBEntityByName(nodeA->getName())->getHealth() << std::endl;
        			
        		}else if (nodeA->getName().find("wolf") == 0){
        			std::cout << nodeA->getName() << std::endl;
        			int points = getOBEntityByName(nodeA->getName())->decreaseHealth();
        			*_pointsPtr = *_pointsPtr + points;
        			std::cout << points << std::endl;
        			std::cout << getOBEntityByName(nodeA->getName())->getHealth() << std::endl;
        			
        		}
        		_rocks->push_back(node->getName());
    		}
    	}
    }
  } 
  } else{
  		PlayState::getSingletonPtr()->isFinalGame();
  }
}

OBEntity* PhysicsController::getOBEntityByName(std::string name){
	OBEntity *obAux = new OBEntity("name");
	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		obAux = *it; 
		if (obAux->getType().compare(name)==0){
			return obAux;
		}
	}
}