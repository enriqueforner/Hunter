
#include "PhysicsController.h"

#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"   
#include "OgreBulletCollisionsRay.h"

#include <iostream>
#include <vector>
#include <algorithm>

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

PhysicsController::PhysicsController(Ogre::SceneManager *sceneMgr, OgreBulletDynamics::DynamicsWorld *world, MovementController * movementController, std::vector <OBEntity*> *obEntities){
	_sceneMgr = sceneMgr;
	_world = world;
	_movementController = movementController;
	_obEntities = obEntities;
	_firstCol = true;
	_latestNodeCol1 = "none";
	_latestNodeCol2 = "none";

}

//~PhysicsController(){}

Ogre::SceneManager *PhysicsController::getSceneManager(){
	return _sceneMgr;
}
OgreBulletDynamics::DynamicsWorld *PhysicsController::getWorld(){
	return _world;
}
MovementController *PhysicsController::getMovementController(){
	return _movementController;
}
// std::vector <OBEntity*> *PhysicsController::getOBEntities(){
// 	return _obEntities;
// }

void PhysicsController::setSceneManager(Ogre::SceneManager *sceneMgr){
	_sceneMgr = sceneMgr;
}
void PhysicsController::setWorld(OgreBulletDynamics::DynamicsWorld *world){
	_world = world;
}
void PhysicsController::setMovementController(MovementController *movementController){
	_movementController = movementController;
}
// void PhysicsController::setOBEntities(std::vector <OBEntity*> *obEntities){
// 	_obEntities = obEntities;
// }


/*void PhysicsController::detectOBEntityCollision(OBEntity *OBEntity){  //hay que ponerle todos los casos, colisiones de objetos de X tipo con X tipo y tal
  //Ogre::Vector2 colliders (0,0);
  bool A = false;
  bool B = false;
  int discrepancies = 0;
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
    //Ogre::SceneNode* drain = _sceneMgr->getSceneNode("CerdoMaloE");
    Ogre::SceneNode* drain = _sceneMgr->getSceneNode(OBEntity->getType());  //Drain es el objeto del que nos interesan las colisiones
    Ogre::Vector3 vect (1,0,0);

    OgreBulletCollisions::Object *obDrain = _world->findObject(drain);  
    
    OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
    OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

    if ((obOB_A == obDrain) || (obOB_B == obDrain)) {  //Si se detecta colision con el objeto del que nos interesa 
      /*std::cout << "HASTA AQUI TODO VA BIEN" << std::endl;
      std::string nameA = obOB_A->getRootNode()->getName().substr(0,3);
      std::cout << "nameA "<< nameA << std::endl;
      std::string nameB = obOB_B->getRootNode()->getName().substr(0,3);
      std::cout << "nameB "<< nameB << std::endl;
      	//Cambiar todos los tipos de obentity a cosas de 4 letras: wolf, rock, rpig (o boar). Creo que no va a hacer falta
      	Ogre::SceneNode* node = NULL;
      	std::cout << "COGEMOS NODE" << std::endl;
	    if ((obOB_A != obDrain) && (obOB_A)) {
	      node = obOB_A->getRootNode(); 
	      bool A = true;
	      /*delete obOB_A;
	    }
	    else if ((obOB_B != obDrain) && (obOB_B)) {
	      node = obOB_B->getRootNode(); 
	      bool B = true;
	      /*delete obOB_B;
	    }

	    if(node){
	      std::string nodeType = node->getName().substr(0,3); 	
	      //std::cout << "nodeType "<< nodeType << std::endl;
	      if(OBEntity->getType().find(nodeType) != 0){ //Si el tipo de los 2 nodos no es el mismo
	      	//El siguiente trozo de codigo sirve para detectar solo el primer "golpe" de una colision
	      	if(node->getName().compare("Redil")!=0){  
	          if(_latestNodeCol1.compare(node->getName())!=0){
	          	discrepancies ++;
	          }
	          if(_latestNodeCol2.compare(OBEntity->getType())!=0){
	          	discrepancies ++;
	          }
	          if(_latestNodeCol1.compare(OBEntity->getType())!=0){
	          	discrepancies ++;
	          }
	          if(_latestNodeCol2.compare(node->getName())!=0){
	          	discrepancies ++;
	          }    
	          if(discrepancies > 2){
	          	_firstCol = true;
	          }
	          _latestNodeCol1 = node->getName();
	          _latestNodeCol2 = OBEntity->getType();
	          if (_firstCol) {
	            std::cout << "DETECTED COLLISION: " << OBEntity->getType() <<" collided with "<< node->getName() << std::endl; 
	            if(node->getName().find("rock")==0){
	    	      //delete node;
	      		  //std::cout << "Node es un rock" << std::endl;
	    	      //deleteNode(node->getName());
	    	      //return;
	    	    }
	    	    else if(OBEntity->getType().find("rock")==0){
	    	      //delete node;
	    		  //std::cout << "OBEntity es un rock" << std::endl;
	    	      //deleteOBEntity(OBEntity);
	    	      //return;
	    	    }
	            _firstCol = false;
	            //delete node;
	          }

	          if(A){
	            delete obOB_A;
	            A = false;
	          }
	          if(B){
	           	delete obOB_B;
	           	B = false;
	          }
	          if(node){
	          	std::cout << "if node" << std::endl;
	            //_sceneMgr->getRootSceneNode()->removeAndDestroyChild(node->getName());
	            _sceneMgr->getRootSceneNode()->removeAndDestroyChild(node->getName());
	            /*_sceneMgr->getRootSceneNode()->getChild(node->getName());
	            _sceneMgr->getRootSceneNode()->removeChild(node->getName());
	            std::cout << "nodo destruido" << std::endl;
	          }
	        }
	        /*if (node && node->getName().compare("Redil")==0){
	          /*std::cout<< "ANTES "<<_movementController->getOBEntityByType(OBEntity->getType())->getRigidBody()->getLinearVelocity() << std::endl;
	          _movementController->moveOne(_movementController->getOBEntityByType(OBEntity->getType()), &vect);
	          std::cout<< "DESPUES "<<_movementController->getOBEntityByType(OBEntity->getType())->getRigidBody()->getLinearVelocity() << std::endl;
	          
	        }
	      	
	      }	
	      
	    }
	    
    }
    //} 
  }
  //return colliders;
}
*/
//std::vector <OBEntity> _obEntities; //Enemigos, proyectiles, escenario...


void PhysicsController::detectCollision(){
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
    /*for (int i = 0; i < 5; ++i){
        
    	std::ostringstream os;
    	os << "Wolf" << i;
    
    	Ogre::SceneNode* drain = _sceneMgr->getSceneNode(os.str());

    	OgreBulletCollisions::Object *obDrain = _world->findObject(drain);  
    
    	OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
    	OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);
    
    	if ((obOB_A == obDrain) || (obOB_B == obDrain)) {
    	  Ogre::SceneNode* node = NULL;
      	  if ((obOB_A != obDrain) && (obOB_A)) {
            node = obOB_A->getRootNode(); delete obOB_A;
          }
          else if ((obOB_B != obDrain) && (obOB_B)) {
            node = obOB_B->getRootNode(); delete obOB_B;
          }
          //El siguiente trozo de codigo sirve para detectar solo el primer "golpe" de una colision
      	  if(node){
        
            std::cout << "DETECTED COLLISION: " << node->getName() << std::endl; 
         
            std::cout << node->getName() << std::endl;
            _sceneMgr->getRootSceneNode()->removeAndDestroyChild (node->getName());
         
          }
        }
    //} 
    }*/

    Ogre::SceneNode* nodeA = NULL;
    Ogre::SceneNode* nodeB = NULL;
    
    OgreBulletCollisions::Object *obOB_A = _world->findObject(obA);
    OgreBulletCollisions::Object *obOB_B = _world->findObject(obB);

    nodeA = obOB_A->getRootNode();
    nodeB = obOB_B->getRootNode();

    if(nodeA && nodeB){
    	if((nodeA->getName().find("wolf") == 0)||(nodeB->getName().find("wolf") == 0)){
    		Ogre::SceneNode* node = NULL;
    		if(nodeA->getName().find("rock") == 0){
    			node = obOB_A->getRootNode();
    			delete obOB_A;
    		}
    		else if(nodeB->getName().find("rock") == 0){
    			node = obOB_B->getRootNode();
    			delete obOB_B;
    		}
    		if(node){
    			std::cout << "DETECTED COLLISION: " << node->getName() << std::endl; 
        		std::cout << node->getName() << std::endl;
        		_sceneMgr->getRootSceneNode()->removeAndDestroyChild (node->getName());
    		}
    	}
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
		*obAux = **it; //Igual aqui peta, por cacharreo intenso de punteros
		std::cout << obAux->getType() << " " << obAux->getIndex()<<std::endl;
	}

	index = 0;
	obAux = new OBEntity("none");
	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		*obAux = **it; //Igual aqui peta, por cacharreo intenso de punteros
		obAux->setIndex(index);
		index ++;
	}
	std::cout <<"indexes reasigned" << std::endl;

	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		*obAux = **it; //Igual aqui peta, por cacharreo intenso de punteros
		std::cout << obAux->getType() << " " << obAux->getIndex()<<std::endl;
	}
}


