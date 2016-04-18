#ifndef FinalState_H
#define FinalState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include <CEGUI.h>

#include "ScenePlayFinal.h"
class FinalState : public Ogre::Singleton<FinalState>, public GameState
{
 public:
  FinalState();

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

  // Heredados de Ogre::Singleton.
  static FinalState& getSingleton ();
  static FinalState* getSingletonPtr ();
  void DestroyAllAttachedMovableObjects(Ogre::SceneNode* i_pSceneNode);
  bool actualizarranking();
  bool exitButtonC(const CEGUI::EventArgs& e);
  bool mainmenuButtonC(const CEGUI::EventArgs& e);

  void LimpiarTodo();

 protected:
 
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;
  ScenePlayFinal* _sPF;
  bool _exitGame;
};

#endif
