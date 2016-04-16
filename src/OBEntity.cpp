#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"    
#include "OgreBulletCollisionsRay.h"

#include "OBEntity.h"

#define WOLF_MAXHEALTH 2
#define WOLF_POINTS 100
#define WOLF_FLEE 500

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
int OBEntity::getIndex(){
	return _index;
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
void OBEntity::setIndex(int index){
	_index = index;
}

int OBEntity::decreaseHealth(){
	int res = 0;
	if((_type.compare("wolf") == 0)  && _health > 0){
		_health = _health - 1;
		//A cada pedrada que le atines, que te de puntos
		_points = _points + WOLF_POINTS;
		res = res + WOLF_POINTS;
		if(_health == 0){
			//Hacer que el lobo huya (o si no nos da tiempo, que desaparezca)
			_points = _points + WOLF_FLEE;
			res = res + WOLF_FLEE;
		}
	}
	return res; //Es el numero de puntos que le tenemos que sumar a la puntuacion del juego
}

bool OBEntity::operator== (OBEntity &obEntity){
	std::cout << "OPERADOR CAMBIADO" << std::endl;
	std::string type1 = "";
	type1 = obEntity.getType().substr(0,3);
	if(_type.find(type1)!= 0){
		return true;
	}
	return false;
}