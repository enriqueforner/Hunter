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
		PhysicsController(Ogre::SceneManager *sceneMgr, OgreBulletDynamics::DynamicsWorld *world, MovementController *movementController, std::vector <OBEntity*> *obEntities); //std::vector <OBEntity*> *obEntities
		//~PhysicsController();
		void detectCollision();
		Ogre::SceneManager *getSceneManager();
		OgreBulletDynamics::DynamicsWorld *getWorld();
		MovementController *getMovementController();
		//std::vector <OBEntity*> *getOBEntities();
		void setSceneManager(Ogre::SceneManager *sceneMgr);
		void setWorld(OgreBulletDynamics::DynamicsWorld *world);
		void setMovementController(MovementController *movementController);
		//void setOBEntities(std::vector <OBEntity*> *obEntities);
		bool _firstCol;
		std::string _latestNodeCol1;
		std::string _latestNodeCol2;
		void deleteNode(std::string name);
		void deleteOBEntity(OBEntity *OBEntity);
		void reasignIndexes();
		OBEntity* getOBEntitieByName(std::string name);

		bool _finalGame;
	private:
		std::vector <OBEntity*> *_obEntities; //Enemigos, proyectiles, escenario...
		Ogre::SceneManager *_sceneMgr;
		OgreBulletDynamics::DynamicsWorld *_world;
		MovementController *_movementController;
		//std::vector <OBEntity*> *_obEntities;
		std::vector<String> *_rocks;

};

#endif