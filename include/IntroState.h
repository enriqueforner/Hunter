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

#ifndef IntroState_H
#define IntroState_H

#include <Ogre.h>
#include <OIS/OIS.h>

#include "GameState.h"
#include "IntroScene.h"

#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>


class IntroState : public Ogre::Singleton<IntroState>, public GameState
{
 public:
  IntroState();

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

  // Heredados de Ogre::Singleton.
  static IntroState& getSingleton ();
  static IntroState* getSingletonPtr ();

  bool exitButtonC(const CEGUI::EventArgs& e);
  bool playButtonC(const CEGUI::EventArgs& e);
  bool creditsButtonC(const CEGUI::EventArgs& e);
  bool rankingButtonC(const CEGUI::EventArgs& e);
  bool retroceder_c(const CEGUI::EventArgs& e);
  bool retroceder_r(const CEGUI::EventArgs& e);
  bool _lanzaranimacion;
  CEGUI::Renderer * getRenderer();

 protected:
  Ogre::Root* _root;
  Ogre::SceneManager* _sceneMgr;
  Ogre::Viewport* _viewport;
  Ogre::Camera* _camera;

  void loadCEGUI();
  CEGUI::MouseButton convertMouseButton(OIS::MouseButtonID id);
  Ogre::AnimationState *_animState;
  bool _exitGame;
  IntroScene* _iS;
  CEGUI::OgreRenderer* renderer;
  CEGUI::Window* _sheet;
};

#endif
