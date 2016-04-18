/*********************************************************************
 * Módulo 1. Curso de Experto en Desarrollo de Videojuegos
 * Autor: David Vallejo Fernández    David.Vallejo@uclm.es
 *
 * Código modificado a partir de Managing Game States with OGRE
 * http://www.ogre3d.org/tikiwiki/Managing+Game+States+with+OGRE
 * Inspirado en Managing Game States in C++
 * http://gamedevgeek.com/tutorials/managing-game-states-in-c/
 *
 * You can redistribute and/or modify this file under the terms of the
 * GNU General Public License ad published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * and later version. See <http://www.gnu.org/licenses/>.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.  
 *********************************************************************/

#ifndef PlayState_H
#define PlayState_H

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

#include <OBEntity.h> 
#include "MovementController.h"
#include "PhysicsController.h"

#include <iostream>
#include <vector>
#include <string>

#include "FinalState.h" 
 
#include "ScenePlayFinal.h"

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

class PlayState : public Ogre::Singleton<PlayState>, public GameState
{
 public:

  PlayState ();
  ~PlayState ();

  void enter ();
  void exit ();
  void pause ();
  void resume ();

  void keyPressed (const OIS::KeyEvent &e);
  void keyReleased (const OIS::KeyEvent &e);

  void mouseMoved (const OIS::MouseEvent &e);
  void mousePressed (const OIS::MouseEvent &e, OIS::MouseButtonID id);
  void mouseReleased (const OIS::MouseEvent &e, OIS::MouseButtonID id);

  bool frameStarted (const Ogre::FrameEvent& evt);
  bool frameEnded (const Ogre::FrameEvent& evt);

  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);
  void CreateInitialWorld();
  //void AddDynamicObject(TEDynamicObject tObject);
  void AddAndThrowDynamicObject(std::string type, double force);
  //RigidBody* pickBody (Vector3 &p, Ray &r, float x, float y);

  // Heredados de Ogre::Singleton.
  static PlayState& getSingleton ();
  static PlayState* getSingletonPtr ();
  void ColocarWolfAndRedilAndPig();
  void DetectCollisionPig();
  void TEDynamicObjectMovement();
  void RecorreVectorTAOAnadirMovimientoConstante();
  void CreationWolf();
  void CrearBosqueAndColina();
  void isFinalGame();
  void lifeWolf();


 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  OIS::Keyboard* _keyboard;
  OIS::Mouse* _mouse;

  OgreBulletDynamics::DynamicsWorld *_world;
  OgreBulletCollisions::DebugDrawer *_debugDrawer;
  int _numEntities;
  float _timeLastObject;

  std::deque <OgreBulletDynamics::RigidBody*>         _bodies;
  std::deque <OgreBulletCollisions::CollisionShape*>  _shapes;
  std::vector <OBEntity*>  _obEntities;
  
  bool _exitGame;
  Ogre::Real _deltaT;
  double _keyDownTime;
  bool _shootKeyDown;
  bool _firstCol;
  std::string _latestNodeCol;
  bool _finalGame;
  bool _lanzaranimationPig;
  Ogre::Camera* _aerialCamera;
  Ogre::Camera* _projectileCamera;
  OgreBulletDynamics::RigidBody* _trackedBody;
  Ogre::Vector2 _mouseRotation;

  MovementController *_movementController;
  PhysicsController *_physicsController;

  std::vector <OgreBulletDynamics::RigidBody*>         _bodiesC;
  std::vector <OgreBulletCollisions::CollisionShape*>  _shapesC;

  ScenePlayFinal* _sPF;

  int _forcePercent;
  int _points;

  std::vector<Ogre::AnimationState*> *_vector_anims_pig;

};

#endif
