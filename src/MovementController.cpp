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
}
	
MovementController::MovementController(Ogre::SceneManager* sceneMgr, std::vector <OBEntity *>  *obEntities){
	_sceneMgr = sceneMgr;
	_obEntities = obEntities;
}

//MovementController::~MovementController(){}

std::vector <OBEntity *> *MovementController::getOBEntities(){
	return _obEntities;
}

void MovementController::setOBEntities(std::vector <OBEntity *> *obEntities){
	_obEntities = obEntities;
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
    OBEntity *obAux = new OBEntity("none");
    Ogre::Vector3 *speed = new Ogre::Vector3(0,0,0);
    Ogre::Vector3 *targetAux = new Ogre::Vector3(0,0,0); 
    Ogre::Vector3 *originAux = new Ogre::Vector3(0,0,0);      

    
    for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
    	obAux = *it; 
    	if(obAux->getType().find("wolf") != std::string::npos){
        	if(obAux->getHealth()>0){
              *targetAux = _sceneMgr->getSceneNode("Redil")->getPosition(); 
              *originAux = obAux->getSceneNode()->getPosition();
              speed = getResultVector(originAux, targetAux, NPC_SPEED); 
              Ogre::Vector3 *vect1 = new Ogre::Vector3(1,0,0);
              moveOne(obAux,vect1);
          }else{
          	  btQuaternion quat;
          	  btTransform transf = obAux->getRigidBody()->getCenterOfMassTransform();
          	  quat = transf.getRotation();
          	  quat.setEuler(0,180,0);
              *targetAux = _sceneMgr->getSceneNode("Redil")->getPosition(); 
              *originAux = obAux->getSceneNode()->getPosition();
              speed = getResultVector(originAux, targetAux, NPC_SPEED); 
              Ogre::Vector3 *vect1 = new Ogre::Vector3(-1,0,0);
              moveOne(obAux,vect1);
          }
        }
        else if(obAux->getType().find("pig") != std::string::npos){
        	moveOne(obAux,speed);
        }
    }
}

void MovementController::moveOne(OBEntity *obAux, Ogre::Vector3 *speed){
	obAux->getRigidBody()->setLinearVelocity(*speed);
}
OBEntity * MovementController::getOBEntityByType(std::string type){
	OBEntity *obAux = new OBEntity("none");
	for(std::vector<OBEntity *>::iterator it = _obEntities->begin(); it != _obEntities->end(); ++it) {
		obAux = *it; 
		if(obAux->getType().compare(type)==0){
			return obAux;
		}
	}
	obAux = new OBEntity("none");	
	return obAux;
}

