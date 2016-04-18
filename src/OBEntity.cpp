#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"    
#include "OgreBulletCollisionsRay.h"

#include "OBEntity.h"

#define WOLF_MAXHEALTH 2
#define WOLF_POINTS 100
#define WOLF_FLEE 250

OBEntity::OBEntity(std::string type){
	_type = type;
	if(_type.find("wolf") == 0){
		_health = WOLF_MAXHEALTH;
		//_points = WOLF_POINTS;
	}
	else{
		_health = 0;
		//_points = 0;
	}
	_points = 0;
}


OBEntity::~OBEntity(){
	delete _sNode;
	delete _collShape;
	delete _rigBody;
}

Ogre::SceneNode* OBEntity::getSceneNode(){
	return _sNode;
}
std::string OBEntity::getType(){
	return _type;
}
OgreBulletCollisions::CollisionShape* OBEntity::getCollisionShape(){
	return _collShape;
}
OgreBulletDynamics::RigidBody* OBEntity::getRigidBody(){
	return _rigBody;
}
int OBEntity::getPoints(){
	return _points;
}
int OBEntity::getHealth(){
	return _health;
}

void OBEntity::setSceneNode(Ogre::SceneNode* sceneNode){
	_sNode = sceneNode;
}
void OBEntity::setType(std::string type){
	_type = type;
}
void OBEntity::setCollisionShape(OgreBulletCollisions::CollisionShape* collisionShape){
	_collShape = collisionShape;
}
void OBEntity::setRigidBody(OgreBulletDynamics::RigidBody* rigidBody){
	_rigBody = rigidBody;
}
void OBEntity::setPoints(int points){
	_points = points;
}

int OBEntity::decreaseHealth(){
	int res = 0;
	if((_type.find("wolf") == 0)  && _health > 0){
		std::cout << "Bajando" << std::endl;
		_health--;
		//A cada pedrada, te da puntos
		_points = _points + WOLF_POINTS;
		res = res + WOLF_POINTS;
		if(_health == 0){
			//Si el lobo huye, te da puntos tambien
			_points = _points + WOLF_FLEE;
			res = res + WOLF_FLEE;
		}
	}
	return res;
}