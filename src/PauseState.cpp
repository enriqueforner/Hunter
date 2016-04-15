
#include "PauseState.h"
#include "PlayState.h"
template<> PauseState* Ogre::Singleton<PauseState>::msSingleton = 0;


void
PauseState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0.0));
  _pS = new PauseScene(_sceneMgr);
  _pS -> crearCegui();
  _exitGame = false;
}

void
PauseState::exit ()
{
  
}

void
PauseState::pause ()
{
}

void
PauseState::resume ()
{
}

bool
PauseState::frameStarted
(const Ogre::FrameEvent& evt)
{
  return true;
}

bool
PauseState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
PauseState::keyPressed
(const OIS::KeyEvent &e) {
  // Tecla p --> Estado anterior.
  if (e.key == OIS::KC_P) {
     _pS->limpiarpantallaCEGUI();
    if(_pS->destroyCegui()){
      popState();
    }
  }
}

void
PauseState::keyReleased
(const OIS::KeyEvent &e)
{
}

void
PauseState::mouseMoved
(const OIS::MouseEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);
}

void
PauseState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
   CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertMouseButton(id));
}

void
PauseState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertMouseButton(id));
}

PauseState*
PauseState::getSingletonPtr ()
{
return msSingleton;
}

PauseState&
PauseState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

CEGUI::MouseButton PauseState::convertMouseButton(OIS::MouseButtonID id)
{
  CEGUI::MouseButton ceguiId;
  switch(id)
    {
    case OIS::MB_Left:
      ceguiId = CEGUI::LeftButton;
      break;
    case OIS::MB_Right:
      ceguiId = CEGUI::RightButton;
      break;
    case OIS::MB_Middle:
      ceguiId = CEGUI::MiddleButton;
      break;
    default:
      ceguiId = CEGUI::LeftButton;
    }
  return ceguiId;
}

bool PauseState::continueButtonC(const CEGUI::EventArgs& e){
    _pS->limpiarpantallaCEGUI();
    if(_pS->destroyCegui()){
       popState();
    }
    return true; 
}