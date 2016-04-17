#include "MovementController.h"

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
#define NPC_SPEED 15.0
 
MovementController::MovementController(Ogre::SceneManager* sceneMgr){
	_sceneMgr = sceneMgr;
	//Crear los 2 nodos guia
	Entity *wolfGuideEntity = _sceneMgr->createEntity("wolfGuideEntity","Lobo.mesh");
  	_wolfGuide = _sceneMgr->createSceneNode("wolfGuide");
  	_wolfGuide->attachObject(wolfGuideEntity);

  	Entity *pigGuideEntity = _sceneMgr->createEntity("pigGuideEntity","CerdoIni.mesh");
  	_pigGuide = _sceneMgr->createSceneNode("pigGuide");
  	_pigGuide->attachObject(pigGuideEntity);

  	_sceneMgr->getRootSceneNode()->addChild(_pigGuide);
  	_sceneMgr->getRootSceneNode()->addChild(_wolfGuide);

  	_pigGuide->setPosition(-20,0,10);
  	//_wolfGuide->setPosition(20,0,0);

  	//_wolfGuide->setVisible(false);
  	//_pigGuide->setVisible(false);

  	_wolfGuideTarget =_sceneMgr->getSceneNode("Redil")->getPosition();
  	_wolfGuide->setPosition(_wolfGuideTarget);
  	std::cout<< "POSICION DEL REDIL: "<<_wolfGuideTarget <<std::endl;
  	//Inicializar la ruta del guia cerdo aqui. Ruta que acabe en el mismo punto que empieza
  	_pigGuidePath = new std::vector <Ogre::Vector3>;
  	_pigGuidePath->push_back(_wolfGuideTarget); //Esto es de prueba, borrar luego
}
	
MovementController::MovementController(Ogre::SceneManager* sceneMgr, std::deque <OgreBulletDynamics::RigidBody *> *bodies, std::deque <OgreBulletCollisions::CollisionShape *> *shapes, std::vector <OBEntity *>  *obEntities){
	_sceneMgr = sceneMgr;
	_bodies = bodies;
	_shapes = shapes;
	_obEntities = obEntities;
	//Crear los 2 nodos guia
	
	Entity *wolfGuideEntity = _sceneMgr->createEntity("wolfGuideEntity","Lobo.mesh");
  	_wolfGuide = _sceneMgr->createSceneNode("wolfGuide");
  	_wolfGuide->attachObject(wolfGuideEntity);

  	Entity *pigGuideEntity = _sceneMgr->createEntity("pigGuideEntity","CerdoIni.mesh");
  	_pigGuide = _sceneMgr->createSceneNode("pigGuide");
  	_pigGuide->attachObject(pigGuideEntity);

  	_sceneMgr->getRootSceneNode()->addChild(_pigGuide);
  	_sceneMgr->getRootSceneNode()->addChild(_wolfGuide);

  	_pigGuide->setPosition(-20,0,10);
  	//_wolfGuide->setPosition(20,0,0);

  	//_wolfGuide->setVisible(false);
  	//_pigGuide->setVisible(false);

  	_wolfGuideTarget =_sceneMgr->getSceneNode("Redil")->getPosition();
  	_wolfGuide->setPosition(_wolfGuideTarget);
  	std::cout<< "POSICION DEL REDIL: "<<_wolfGuideTarget <<std::endl;
  	//Inicializar la ruta del guia cerdo aqui. Ruta que acabe en el mismo punto que empieza
  	_pigGuidePath = new std::vector <Ogre::Vector3>;
  	_pigGuidePath->push_back(_wolfGuideTarget); //Esto es de prueba, borrar luego
}

//MovementController::~MovementController(){}

std::deque <OgreBulletDynamics::RigidBody *> *MovementController::getRigidBodies(){
	return _bodies;
}
std::deque <OgreBulletCollisions::CollisionShape *>  *MovementController::getCollisionShapes(){
	return _shapes;
}
std::vector <OBEntity *> *MovementController::getOBEntities(){
	return _obEntities;
}
Ogre::SceneNode *MovementController::getWolfGuideSN(){
	return _wolfGuide;
}
Ogre::SceneNode *MovementController::getPigGuideSN(){
	return _pigGuide;
}

void MovementController::setRigidBodies(std::deque <OgreBulletDynamics::RigidBody *> *bodies){
	_bodies = bodies;
}

void MovementController::setCollisionShapes(std::deque <OgreBulletCollisions::CollisionShape *> *shapes){
	_shapes = shapes;
}

void MovementController::setOBEntities(std::vector <OBEntity *> *obEntities){
	_obEntities = obEntities;
}
void MovementController::setWolfGuideSN(Ogre::SceneNode *wolfGuide){
	_wolfGuide = wolfGuide;
}
void MovementController::setPigGuideSN(Ogre::SceneNode *pigGuide){
	_pigGuide = pigGuide;
}
void MovementController::setSceneManager(Ogre::SceneManager *sceneMgr){
	_sceneMgr = sceneMgr;
}

Ogre::Vector3 *MovementController::getResultVector(Ogre::Vector3 *origin, Ogre::Vector3 *end, double scale){
	int x = end->x - origin->x;
	int y = end->y - origin->y;
	int z = end->z - origin->z;
	Ogre::Vector3 *result = new Ogre::Vector3(scale*x,scale*y,scale*z);
	return result;
}

void MovementController::moveAll(){
	/*MOVIMIENTO DE PRUEBA
	for(std::deque<OgreBulletDynamics::RigidBody *>::iterator it = _bodies->begin(); it != _bodies->end(); ++it) {
          (*it) -> setLinearVelocity(Ogre::Vector3(0,0,1));  
    }*/
    OBEntity *obAux = new OBEntity("none");
    Ogre::Vector3 *speed = new Ogre::Vector3(0,0,0);
    Ogre::Vector3 *targetAux = new Ogre::Vector3(0,0,0); 
    Ogre::Vector3 *originAux = new Ogre::Vector3(0,0,0);      

    if(_pigGuidePath->size() < 1){
    	//Volver a meterle la ruta
    }

    //con OBEntities
    for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
    	*obAux = **it; //Igual aqui peta, por cacharreo intenso de punteros
    	//std::cout<< obAux->getType() << std::endl;
    	//std::cout << obAux->getType() <<std::endl;
        //if(obAux->getType() == "wolf"){
    	if(obAux->getType().find("wolf") != std::string::npos){
        	//std::cout << "ES UN WOLF" <<std::endl;
        	//*targetAux = _wolfGuideTarget; 
        	*targetAux = _sceneMgr->getSceneNode("Redil")->getPosition(); 
        	*originAux = obAux->getSceneNode()->getPosition();
        	speed = getResultVector(originAux, targetAux, NPC_SPEED); 
        	//speed = new Ogre::Vector3(1,0,0);
        	//speed->normalise(); //???
        	moveOne(obAux,speed);
        }
        else if(obAux->getType().find("pig") != std::string::npos){
        //else if(obAux->getType() == "pig"){
        	//std::cout << "ES UN PIG" <<std::endl;
        	//*targetAux = _pigGuide->getPosition();
        	//*originAux = obAux->getSceneNode()->getPosition();
        	//speed = getResultVector(originAux, targetAux, NPC_SPEED);
        	//speed = new Ogre::Vector3(0,0,1);
        	//speed->normalise(); //???
        	moveOne(obAux,speed);
        	//ver como queda. Si no, tener un vector con los pigs y que el primero siga al guia, el segundo al primero y asi
        	//otra opcion, varios pigGuides (2 o 3) y que cada pig siga a uno al azar

        }

        // (*it) ->getRigidBody()->setLinearVelocity(Ogre::Vector3(0,0,1));  
    }
}

void MovementController::moveOne(OBEntity *obAux, Ogre::Vector3 *speed){
	Ogre::Vector3 vect (0,0,1);
	//if(obAux->getRigidBody()->getLinearVelocity() != vect){
	if(*speed != vect){
		obAux->getRigidBody()->setLinearVelocity(*speed);
	}
	Ogre::Vector3 vect1 (1,0,1);
	obAux->getRigidBody()->setLinearVelocity(vect1);

	/*else if(obAux->getRigidBody()->getLinearVelocity() != *speed){
		obAux->getRigidBody()->setLinearVelocity(*speed);
	}*/
	
	// Ogre::Vector3 vel (1,0,0);
	// obAux->getRigidBody()->setLinearVelocity(vel);
}
OBEntity * MovementController::getOBEntityByType(std::string type){
	OBEntity *obAux = new OBEntity("none");
	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		*obAux = **it; //Igual aqui peta, por cacharreo intenso de punteros
		if(obAux->getType().compare(type)==0){
			return obAux;
		}
	}
	obAux = new OBEntity("none");	
	return obAux;
}

