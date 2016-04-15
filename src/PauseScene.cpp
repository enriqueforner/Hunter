//PauseScene.cpp

#include "PauseScene.h"
#include "PauseState.h"

PauseScene::PauseScene(Ogre::SceneManager *sceneManager){
  _sheet = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  _sceneManager = sceneManager;
}

PauseScene::~PauseScene(){}


void PauseScene::crearCegui(){

	CEGUI::Window* vent = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("pausesimple.layout");
	vent->setPosition(CEGUI::UVector2(CEGUI::UDim(0.13f,0),CEGUI::UDim(0.16f,0)));
	_sheet -> addChild(vent);

	CEGUI::Window* continueButton = vent->getChild("Continue");
    continueButton ->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&PauseState::continueButtonC, 
                PauseState::getSingletonPtr())); 
    /*
    CEGUI::Window* imgnamepause = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","InamePause");
  	CEGUI::ImageManager::getSingleton().addFromImageFile("InamePause","pausename.png");
  	imgnamepause -> setProperty("Image","InamePause");
  	imgnamepause -> setProperty("BackgroundEnabled","False");
  	imgnamepause -> setProperty("FrameEnabled","False");
  	imgnamepause -> setSize(CEGUI::USize(CEGUI::UDim(0.19f,0),CEGUI::UDim(0.22f,0)));  
  	imgnamepause -> setPosition(CEGUI::UVector2(CEGUI::UDim(0.31f,0),CEGUI::UDim(0.09f,0)));
   	vent -> addChild(imgnamepause);              
	*/
}

bool PauseScene::destroyCegui(){
	//_sheet-> getChild("PauseSimple")->destroyChild("InamePause");
	//CEGUI::ImageManager::getSingleton().destroy("InamePause");
	_sheet-> destroyChild("PauseSimple");
	return true;
}

void PauseScene::limpiarpantallaCEGUI(){
	_sheet-> getChild("PauseSimple")->setVisible(false);
}