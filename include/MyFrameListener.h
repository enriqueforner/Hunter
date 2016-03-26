#include <Ogre.h>
#include <OIS/OIS.h>
#include <OgreOverlaySystem.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>

#include <OgreBulletDynamicsRigidBody.h>
#include <Shapes/OgreBulletCollisionsStaticPlaneShape.h>
#include <Shapes/OgreBulletCollisionsBoxShape.h>

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

class MyFrameListener : public Ogre::FrameListener {
private:
  Camera* _camera;
  OverlayManager* _overlayManager;
  SceneManager *_sceneManager;
  RenderWindow *_win;

  OIS::InputManager* _inputManager;
  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;

  OgreBulletDynamics::DynamicsWorld * _world;
  OgreBulletCollisions::DebugDrawer * _debugDrawer;
  int _numEntities;
  float _timeLastObject;

  std::deque <OgreBulletDynamics::RigidBody *>         _bodies;
  std::deque <OgreBulletCollisions::CollisionShape *>  _shapes;

  enum TEDynamicObject {
    box,
    sheep
  };
  
  void CreateInitialWorld();
  void AddDynamicObject(TEDynamicObject tObject);
  RigidBody* pickBody(Vector3 &p, Ray &r, float x, float y);

public:
  MyFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, 
		  Ogre::OverlayManager* om, Ogre::SceneManager* sm);
  ~MyFrameListener();
  bool frameStarted(const Ogre::FrameEvent& evt);  
  bool frameEnded(const Ogre::FrameEvent& evt);  
};
