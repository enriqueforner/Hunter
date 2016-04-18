#ifndef OBEntity_H
#define OBEntity_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"

#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>

#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>

#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>

#include <string>

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

class OBEntity
{
 public:
  OBEntity() {}
  OBEntity(std::string type);
  ~OBEntity();

  Ogre::SceneNode* getSceneNode();
  std::string getType();
  OgreBulletCollisions::CollisionShape* getCollisionShape();
  OgreBulletDynamics::RigidBody* getRigidBody();
  int getPoints();
  int getHealth();

  void setSceneNode(Ogre::SceneNode* sceneNode);
  void setType(std::string type);
  void setCollisionShape(OgreBulletCollisions::CollisionShape* collisionShape);
  void setRigidBody(OgreBulletDynamics::RigidBody* rigidBody);
  void setPoints(int points);
  int decreaseHealth();

 protected:
  Ogre::SceneNode* _sNode;
  std::string _type;
  OgreBulletCollisions::CollisionShape* _collShape;
  OgreBulletDynamics::RigidBody* _rigBody;
  int _points;
  int _health;
};

#endif
