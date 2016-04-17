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
	setPigPath();
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
std::vector<Ogre::Vector3> * OBEntity::getPigPath(){
	return _pigPath;
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
void OBEntity::setPigPath(){
	std::vector<Ogre::Vector3> *path;
	if(_type.find("pig") == 0){
		if (_pigPath->size() == 0){
			//PONER AQUI LOS 4 PUNTOS DONDE VAN A ESTAR DANDO VUELTAS LOS CERDOS
			Ogre::Vector3 vec1(0,0,0);
			Ogre::Vector3 vec2(0,0,0);
			Ogre::Vector3 vec3(0,0,0);
			Ogre::Vector3 vec4(0,0,0);

			path = new std::vector<Ogre::Vector3>;
			path-> push_back(vec1);
			path-> push_back(vec2);
			path-> push_back(vec3);
			path-> push_back(vec4);
			
			_pigPath = path;
		}
	}	
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

/*bool OBEntity::operator== (OBEntity &obEntity){
	std::cout << "OPERADOR CAMBIADO" << std::endl;
	std::string type1 = "";
	type1 = obEntity.getType().substr(0,3);
	if(_type.find(type1)!= 0){
		return true;
	}
	return false;
}*/

 Ogre::Vector3 *OBEntity::getNextPathPoint(){
 	Ogre::Vector3 *vecPos; 
 	if(_type.find("pig") == 0){
		if (_pigPath->size() > 0){
			Ogre::Vector3 vecAux = _pigPath->at(0);
			vecPos = new Ogre::Vector3(vecAux.x,vecAux.y,vecAux.z);
		}
		else{
			setPigPath();
			Ogre::Vector3 vecAux = _pigPath->at(0);
			vecPos = new Ogre::Vector3(vecAux.x,vecAux.y,vecAux.z);
		}
	}
	return vecPos;
 }