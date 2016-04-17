#include "IntroState.h"
#include "PlayState.h"
#include "Ranking.h"

template<> IntroState* Ogre::Singleton<IntroState>::msSingleton = 0;

IntroState::IntroState(){
    _root = Ogre::Root::getSingletonPtr();
    _sceneMgr = _root->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
    _sceneMgr -> setAmbientLight(Ogre::ColourValue(1,1,1));
    _camera = _sceneMgr->createCamera("IntroCamera");
    
    loadCEGUI();
    
}

void
IntroState::enter ()
{

  _sceneMgr -> setAmbientLight(Ogre::ColourValue(1,1,1));
  
  _camera = _sceneMgr->getCamera("IntroCamera");
  _camera->setPosition(Ogre::Vector3(5,20,56));
  _camera->lookAt(Ogre::Vector3(0,0,0));
  _camera->setNearClipDistance(5);
  _camera->setFarClipDistance(10000);  

  _viewport = _root->getAutoCreatedWindow()->addViewport(_camera);
  //_viewport->setBackgroundColour(Ogre::ColourValue(1.0, 1.0, 1.0));
  
  _iS = new IntroScene(_sceneMgr);
  _iS-> crearMenuInicioCEGUI();
  _iS-> crearWorld();
  _iS -> ActRanking();
  _lanzaranimacion = true;
  _animState = NULL;
  _sceneMgr->setSkyBox(true, "MySky");
  _exitGame = false;
}

void
IntroState::exit()
{
  _iS -> destroyCegui();
  _sceneMgr->clearScene();
  _root->getAutoCreatedWindow()->removeAllViewports();
}

void
IntroState::pause ()
{
}

void
IntroState::resume ()
{
}

bool
IntroState::frameStarted
(const Ogre::FrameEvent& evt) 
{
  Ogre::Real deltaT = evt.timeSinceLastFrame;
  if (_lanzaranimacion){
    _animState = _sceneMgr->getEntity("CerdoIni")->getAnimationState("Salto");
    _animState->setEnabled(true);
    _animState->setLoop(true);
    _animState->setTimePosition(0.0);
    _lanzaranimacion = false;
  }
  if (_animState != NULL) {
     if (_animState->hasEnded()) {
       _animState->setTimePosition(0.0);
       _animState->setEnabled(false);
     }
     else {
       _animState->addTime(deltaT);
     }
  }
  return true;
}

bool
IntroState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
IntroState::keyPressed
(const OIS::KeyEvent &e)
{
  // Transición al siguiente estado.
  // Espacio --> PlayState
  if (e.key == OIS::KC_SPACE) {
    changeState(PlayState::getSingletonPtr());
  }
}

void
IntroState::keyReleased
(const OIS::KeyEvent &e )
{
  if (e.key == OIS::KC_ESCAPE) {
    _exitGame = true;
  }
}

void
IntroState::mouseMoved
(const OIS::MouseEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);  
}

void
IntroState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertMouseButton(id)); 
}

void
IntroState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{ 
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertMouseButton(id));
}

IntroState*
IntroState::getSingletonPtr ()
{
return msSingleton;
}

IntroState&
IntroState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

CEGUI::MouseButton IntroState::convertMouseButton(OIS::MouseButtonID id)
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

bool IntroState::exitButtonC(const CEGUI::EventArgs& e){
    _exitGame = true;
    return true; 
}
bool IntroState::playButtonC(const CEGUI::EventArgs& e){
    _iS->limpiarpantallaCEGUI();
    changeState(PlayState::getSingletonPtr());
    return true; 
}
bool IntroState::creditsButtonC(const CEGUI::EventArgs& e){
    _iS->creditos();
    return true; 
}
bool IntroState::rankingButtonC(const CEGUI::EventArgs& e){
    _iS->ranking();
    return true; 
}
bool IntroState::retroceder_c(const CEGUI::EventArgs& e){
    _iS->retroceder();
    return true; 
}
bool IntroState::retroceder_r(const CEGUI::EventArgs& e){
    _iS->retroceder();
    return true; 
}
void IntroState::loadCEGUI(){

  //CEGUI
  renderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  //Para los botones
  CEGUI::SchemeManager::getSingleton().createFromFile("VanillaSkin.scheme");
  //Para el Raton
  CEGUI::SchemeManager::getSingleton().createFromFile("OgreTray.scheme");
  //Para nada de momento
  CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
  
  CEGUI::System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-12");
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("OgreTrayImages/MouseArrow");
  CEGUI::FontManager::getSingleton().createAll("*.font", "Fonts");
  _sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow","Ex1/Sheet");
  CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(_sheet);
  
}

CEGUI::Renderer * IntroState::getRenderer(){
  return renderer;
}
