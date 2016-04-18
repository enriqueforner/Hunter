#include "FinalState.h"
#include "Ranking.h"
#include "IntroState.h"
#include <OgreEntity.h>

template<> FinalState* Ogre::Singleton<FinalState>::msSingleton = 0;

FinalState::FinalState(){}

void
FinalState::enter ()
{
  _root = Ogre::Root::getSingletonPtr();

  // Se recupera el gestor de escena y la cámara.
  _sceneMgr = _root->getSceneManager("SceneManager");
  _camera = _sceneMgr->getCamera("IntroCamera");
  _viewport = _root->getAutoCreatedWindow()->getViewport(0);
  // Nuevo background colour.
  _viewport->setBackgroundColour(Ogre::ColourValue(0.0, 1.0, 0.0));
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setVisible(true);
  _sPF = new ScenePlayFinal(_sceneMgr);
  _sPF -> crearCeguiFinal();
  _exitGame = false;
  
}

void
FinalState::exit ()
{
  //_sceneMgr->destroyCamera("IntroCamera");
  _sceneMgr->destroyCamera("AerialCamera");
  _sceneMgr-> destroyCamera("ProjectileCamera");
  CEGUI::Window* sheet = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  Ogre::Node *node;
  Ogre::Entity *entity;
  Ogre::Node::ChildNodeIterator iter = _sceneMgr->getRootSceneNode()->getChildIterator();
  while (iter.hasMoreElements()){
      node = iter.getNext();
      //DestroyAllAttachedMovableObjects(static_cast<Ogre::SceneNode*>(node));
      static_cast<Ogre::SceneNode*>(node)->removeAndDestroyAllChildren();
      _sceneMgr->getRootSceneNode()->removeAndDestroyChild(node->getName());
  }
  static_cast<Ogre::SceneNode*>(_sceneMgr->getRootSceneNode())->removeAndDestroyAllChildren();
  
  _sPF -> cleanCegui();
  LimpiarTodo();
  _root->getAutoCreatedWindow()->removeAllViewports();

}

void FinalState::DestroyAllAttachedMovableObjects(Ogre::SceneNode* i_pSceneNode )
{
       Ogre::SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

       while ( itObject.hasMoreElements() )
       {
          Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
          //i_pSceneNode->getCreator()->destroyMovableObject( pObject );
          delete pObject;
       }
/*
       Ogre::SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

       while ( itChild.hasMoreElements() )
       {
          Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
          DestroyAllAttachedMovableObjects( pChildNode );
       }
*/
}

void
FinalState::pause ()
{
}

void
FinalState::resume ()
{
}

bool
FinalState::frameStarted
(const Ogre::FrameEvent& evt)
{
  
  return true;
}

bool
FinalState::frameEnded
(const Ogre::FrameEvent& evt)
{
  if (_exitGame)
    return false;
  
  return true;
}

void
FinalState::keyPressed
(const OIS::KeyEvent &e) {
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(e.key));
  CEGUI::System::getSingleton().getDefaultGUIContext().injectChar(e.text);
  
  if (e.key == OIS::KC_ESCAPE) {
    if(actualizarranking()){
        _exitGame = true;
    } 
  }  
   
}

void
FinalState::keyReleased
(const OIS::KeyEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyDown(static_cast<CEGUI::Key::Scan>(e.key));
}

void
FinalState::mouseMoved
(const OIS::MouseEvent &e)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseMove(e.state.X.rel, e.state.Y.rel);
}

void
FinalState::mousePressed
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown(convertMouseButton(id));
}

void
FinalState::mouseReleased
(const OIS::MouseEvent &e, OIS::MouseButtonID id)
{
  CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp(convertMouseButton(id));   
}

CEGUI::MouseButton FinalState::convertMouseButton(OIS::MouseButtonID id)
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

FinalState*
FinalState::getSingletonPtr ()
{
return msSingleton;
}

FinalState&
FinalState::getSingleton ()
{ 
  assert(msSingleton);
  return *msSingleton;
}

bool FinalState::exitButtonC(const CEGUI::EventArgs& e){
    if(actualizarranking()){
        _exitGame = true;
    } 
    return true; 
}

void FinalState::LimpiarTodo(){

  CEGUI::Renderer * renderer = IntroState::getSingletonPtr()->getRenderer();
  renderer-> destroyTexture("InameRanking");
  renderer-> destroyTexture("IpointsRanking");
  renderer-> destroyTexture("INameHunter");


}

bool FinalState::mainmenuButtonC(const CEGUI::EventArgs& e){
    if(actualizarranking()){
        changeState(IntroState::getSingletonPtr());
    } 
    return true; 
}

bool FinalState::actualizarranking(){
    Ranking* ran = new Ranking();
    string puntosplayer = _sPF->getPointsPlayer();
    string nameplayer = _sPF-> getNamePlayer();
    cout << nameplayer << ":" << puntosplayer << endl;
    ran -> setrankingtxt(nameplayer,puntosplayer);
    return true;
}