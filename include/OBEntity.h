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

enum TEDynamicObject {
    box,
    sheep,
    rock,
    wolf
};

class OBEntity
{
 public:
  OBEntity() {}
  OBEntity(std::string type);
  ~OBEntity();

  Ogre::SceneNode* getSceneNode();
  //TEDynamicObject getType();
  std::string getType();
  OgreBulletCollisions::CollisionShape* getCollisionShape();
  OgreBulletDynamics::RigidBody* getRigidBody();
  int getPoints();
  int getIndex();

  void setSceneNode(Ogre::SceneNode* sceneNode);
  //void setType(TEDynamicObject type);
  void setType(std::string type);
  void setCollisionShape(OgreBulletCollisions::CollisionShape* collisionShape);
  void setRigidBody(OgreBulletDynamics::RigidBody* rigidBody);
  void setPoints(int points);
  int decreaseHealth();
  void setIndex(int index);
  bool operator== (OBEntity &obEntity1);

 protected:
  Ogre::SceneNode* _sNode;
  //TEDynamicObject _type;
  std::string _type;
  OgreBulletCollisions::CollisionShape* _collShape;
  OgreBulletDynamics::RigidBody* _rigBody;
  int _points;
  int _health;
  int _index;
};

#endif
