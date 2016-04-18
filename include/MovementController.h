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
		MovementController(Ogre::SceneManager *sceneMgr, std::vector <OBEntity *>  *obEntities);
		//~MovementController();
		void moveAll();
		void moveOne(OBEntity *obAux, Ogre::Vector3 *speed);
		Ogre::Vector3 *getResultVector(Ogre::Vector3 *origin, Ogre::Vector3 *end, double scale);
		std::vector <OBEntity*> *getOBEntities(); 
		void setOBEntities(std::vector <OBEntity*> *obEntities); 
		void setSceneManager(Ogre::SceneManager *sceneMgr);
		OBEntity *getOBEntityByType(std::string type);

	private:
		Ogre::SceneManager *_sceneMgr;
  		std::vector <OBEntity*>  *_obEntities;
};

#endif