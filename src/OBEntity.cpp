#include "Shapes/OgreBulletCollisionsTrimeshShape.h"  
#include "Shapes/OgreBulletCollisionsSphereShape.h" 
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"

#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"    
#include "OgreBulletCollisionsRay.h"

#include "OBEntity.h"

OBEntity::OBEntity(TEDynamicObject type){
	_type = type;
}


OBEntity::~OBEntity(){
	delete _sNode;
	delete _collShape;
	delete _rigBody;
}

Ogre::SceneNode* OBEntity::getSceneNode(){
	return _sNode;
}
TEDynamicObject OBEntity::getType(){
	return _type;
}
OgreBulletCollisions::CollisionShape* OBEntity::getCollisionShape(){
	return _collShape;
}
OgreBulletDynamics::RigidBody* OBEntity::getRigidBody(){
	return _rigBody;
}
void OBEntity::setSceneNode(Ogre::SceneNode* sceneNode){
	_sNode = sceneNode;
}
void OBEntity::setType(TEDynamicObject type){
	_type = type;
}
void OBEntity::setCollisionShape(OgreBulletCollisions::CollisionShape* collisionShape){
	_collShape = collisionShape;
}
void OBEntity::setRigidBody(OgreBulletDynamics::RigidBody* rigidBody){
	_rigBody = rigidBody;
}
