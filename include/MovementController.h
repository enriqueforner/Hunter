#ifndef MovementController_H
#define MovementController_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>

#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>

#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>

#include <OBEntity.h> 

#include <iostream>
#include <vector>

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

class MovementController{
	public:
		MovementController(Ogre::SceneManager *sceneMgr);
		MovementController(Ogre::SceneManager *sceneMgr, std::deque <OgreBulletDynamics::RigidBody*> *bodies, std::deque <OgreBulletCollisions::CollisionShape *> *shapes, std::vector <OBEntity *>  *obEntities);
		//~MovementController();
		void moveAll();
		void moveOne(OBEntity *obAux, Ogre::Vector3 *speed);
		Ogre::Vector3 *getResultVector(Ogre::Vector3 *origin, Ogre::Vector3 *end, double scale);
		std::deque <OgreBulletDynamics::RigidBody*> *getRigidBodies();
		std::deque <OgreBulletCollisions::CollisionShape*>  *getCollisionShapes();
		std::vector <OBEntity*> *getOBEntities(); 
		Ogre::SceneNode *getWolfGuideSN();
		Ogre::SceneNode *getPigGuideSN();
		void setRigidBodies(std::deque <OgreBulletDynamics::RigidBody*> *bodies);
		void setCollisionShapes(std::deque <OgreBulletCollisions::CollisionShape*> *shapes); 
		void setOBEntities(std::vector <OBEntity*> *obEntities); 
		void setWolfGuideSN(Ogre::SceneNode *wolfGuide);
		void setPigGuideSN(Ogre::SceneNode *pigGuide);
		void setSceneManager(Ogre::SceneManager *sceneMgr);
		OBEntity *getOBEntityByType(std::string type);

	private:
		Ogre::SceneManager *_sceneMgr;
		std::deque <OgreBulletDynamics::RigidBody*> *_bodies;
  		std::deque <OgreBulletCollisions::CollisionShape*> *_shapes;
  		std::vector <OBEntity*>  *_obEntities;
  		Ogre::SceneNode *_wolfGuide;
  		Ogre::SceneNode *_pigGuide;
  		Ogre::Vector3 _wolfGuideTarget;
  		std::vector <Ogre::Vector3> *_pigGuidePath;

};

#endif