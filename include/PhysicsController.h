#ifndef PhysicsController_H
#define PhysicsController_H

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

class PhysicsController{
	public:
		PhysicsController();
		PhysicsController(Ogre::SceneManager *sceneMgr, OgreBulletDynamics::DynamicsWorld *world); //std::vector <OBEntity*> *obEntities
		//~PhysicsController();
		Ogre::Vector2 detectCollision();
		Ogre::SceneManager *getSceneManager();
		OgreBulletDynamics::DynamicsWorld *getWorld();
		//std::vector <OBEntity*> *getOBEntities();
		void setSceneManager(Ogre::SceneManager *sceneMgr);
		void setWorld(OgreBulletDynamics::DynamicsWorld *world);
		//void setOBEntities(std::vector <OBEntity*> *obEntities);
		bool _firstCol;
		std::string _latestNodeCol;
	private:
		std::vector <OBEntity> _obEntities; //Enemigos, proyectiles, escenario...
		Ogre::SceneManager *_sceneMgr;
		OgreBulletDynamics::DynamicsWorld *_world;
		//std::vector <OBEntity*> *_obEntities;


};

#endif