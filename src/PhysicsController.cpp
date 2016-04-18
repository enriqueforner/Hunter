
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
	_latestNodeCol1 = "none";
	_latestNodeCol2 = "none";
}

PhysicsController::PhysicsController(Ogre::SceneManager *sceneMgr, OgreBulletDynamics::DynamicsWorld *world, std::vector <OBEntity*> *obEntities, int *pointsPtr){
	_sceneMgr = sceneMgr;
	_world = world;
	//_movementController = movementController;
	_obEntities = obEntities;
	_pointsPtr = pointsPtr;
	_firstCol = true;
	_latestNodeCol1 = "none";
	_latestNodeCol2 = "none";
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
/*MovementController *PhysicsController::getMovementController(){
	return _movementController;
}*/
// std::vector <OBEntity*> *PhysicsController::getOBEntities(){
// 	return _obEntities;
// }

void PhysicsController::setSceneManager(Ogre::SceneManager *sceneMgr){
	_sceneMgr = sceneMgr;
}
void PhysicsController::setWorld(OgreBulletDynamics::DynamicsWorld *world){
	_world = world;
}
/*void PhysicsController::setMovementController(MovementController *movementController){
	_movementController = movementController;
}*/

void PhysicsController::detectCollision(){
  //CODIGO COMENTADO PARA DETECTAR COLISIONES EN LOS DEMAS OBJETOS YA SEA EN MOVIMENTO O QUIETOS
  if (_finalGame){
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
    				//node = obOB_A->getRootNode();
    				//delete obOB_A;
    				if (node->getName().find(*it) == 0){
    					goodCollision = false;
    				}
    			}
    		}
    		else if(nodeB->getName().find("rock") == 0){
    			node = obOB_B->getRootNode();
    			//delete obOB_B;
    			for(std::vector<String>::iterator it = _rocks->begin(); it != _rocks->end(); ++it) {
    				//node = obOB_A->getRootNode();
    				//delete obOB_A;
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
        		//std::cout << node->getName() << std::endl;
        		if (nodeB->getName().find("wolf") == 0){
        			std::cout << nodeB->getName() << std::endl;
        			int points = getOBEntitieByName(nodeB->getName())->decreaseHealth();
        			*_pointsPtr = *_pointsPtr + points;
        			std::cout << points << std::endl;
        			std::cout << getOBEntitieByName(nodeA->getName())->getHealth() << std::endl;
        			
        		}else if (nodeA->getName().find("wolf") == 0){
        			std::cout << nodeA->getName() << std::endl;
        			int points = getOBEntitieByName(nodeA->getName())->decreaseHealth();
        			*_pointsPtr = *_pointsPtr + points;
        			std::cout << points << std::endl;
        			std::cout << getOBEntitieByName(nodeA->getName())->getHealth() << std::endl;
        			
        		}
        		_rocks->push_back(node->getName());
        		//_sceneMgr->getRootSceneNode()->removeAndDestroyChild (node->getName());
    		}
    	}
    }
  } 
  } else{
  		PlayState::getSingletonPtr()->isFinalGame();
  }
}

OBEntity* PhysicsController::getOBEntitieByName(std::string name){
	OBEntity *obAux = new OBEntity("name");
	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		obAux = *it; //Igual aqui peta, por cacharreo intenso de punteros
		if (obAux->getType().compare(name)==0){
			return obAux;
		}
	}
}

void PhysicsController::deleteNode(std::string name){
	std::cout << name << " has entered deletenode"<< std::endl;
	Ogre::SceneNode* auxSN = _sceneMgr->getSceneNode(name);
	std::cout << name << "auxSN cogido"<< std::endl;
	if(auxSN){
		delete auxSN;
		std::cout << name << " node has been deleted"<< std::endl;
	}
	else{
		std::cout << name << " node not found"<< std::endl;
	}
}

void PhysicsController::deleteOBEntity(OBEntity *OBEntity){
	std::string obentityName = "";
	std::cout << OBEntity->getType() << " has entered deleteOBEntity"<< std::endl;
	/*if((std::find(_obEntities->begin(), _obEntities->end(), OBEntity)) == _obEntities->end()){
		std::cout << OBEntity->getType() << " not found"<< std::endl;
	}*/
	obentityName = _obEntities->at(OBEntity->getIndex())->getType();
	if(obentityName.compare(OBEntity->getType()) == 0){
		//El obentity sigue en el vector
		//quitarlo de _obEntities si esta
		_obEntities->erase(_obEntities->begin()+OBEntity->getIndex());
		std::cout <<"obentity has been deleted" << std::endl;
		deleteNode(OBEntity->getType());
		//reordenar los indices de las OBEntities
		reasignIndexes();
	}
	else{
		std::cout << OBEntity->getType() << " not found"<< std::endl;
	}	
}

void PhysicsController::reasignIndexes(){
	int index = 0;
	OBEntity *obAux = new OBEntity("none");
	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		obAux = *it; //Igual aqui peta, por cacharreo intenso de punteros
		std::cout << obAux->getType() << " " << obAux->getIndex()<<std::endl;
	}

	index = 0;
	obAux = new OBEntity("none");
	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		obAux = *it; //Igual aqui peta, por cacharreo intenso de punteros
		obAux->setIndex(index);
		index ++;
	}
	std::cout <<"indexes reasigned" << std::endl;

	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		obAux = *it; //Igual aqui peta, por cacharreo intenso de punteros
		std::cout << obAux->getType() << " " << obAux->getIndex()<<std::endl;
	}
}


