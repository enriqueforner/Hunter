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
#include <MovementController.h>

#include <iostream>
#include <vector>

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

class PhysicsController{
	public:
		PhysicsController();
		PhysicsController(Ogre::SceneManager *sceneMgr, OgreBulletDynamics::DynamicsWorld *world, std::vector <OBEntity*> *obEntities, int *pointsPtr); 
		void detectCollision();
		Ogre::SceneManager *getSceneManager();
		OgreBulletDynamics::DynamicsWorld *getWorld();
		void setSceneManager(Ogre::SceneManager *sceneMgr);
		void setWorld(OgreBulletDynamics::DynamicsWorld *world);
		bool _firstCol;
		OBEntity* getOBEntityByName(std::string name);

		bool _finalGame;
	private:
		std::vector <OBEntity*> *_obEntities; //Enemigos, proyectiles, escenario...
		Ogre::SceneManager *_sceneMgr;
		OgreBulletDynamics::DynamicsWorld *_world;
		MovementController *_movementController;
		std::vector<String> *_rocks;
		int *_pointsPtr;

};

#endif