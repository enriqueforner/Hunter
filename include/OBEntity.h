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

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

enum TEDynamicObject {
    box,
    sheep,
    rock,
    food,
    cow,
    wolf
};

class OBEntity
{
 public:
  OBEntity() {}
  OBEntity(TEDynamicObject type);
  ~OBEntity();

  Ogre::SceneNode* getSceneNode();
  TEDynamicObject getType();
  OgreBulletCollisions::CollisionShape* getCollisionShape();
  OgreBulletDynamics::RigidBody* getRigidBody();

  void setSceneNode(Ogre::SceneNode* sceneNode);
  void setType(TEDynamicObject type);
  void setCollisionShape(OgreBulletCollisions::CollisionShape* collisionShape);
  void setRigidBody(OgreBulletDynamics::RigidBody* rigidBody);

 protected:
  Ogre::SceneNode* _sNode;
  TEDynamicObject _type;
  OgreBulletCollisions::CollisionShape* _collShape;
  OgreBulletDynamics::RigidBody* _rigBody;
};

#endif
