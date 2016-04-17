//ScenePlayFinal.cpp
#include "ScenePlayFinal.h"
#include "FinalState.h"

ScenePlayFinal::ScenePlayFinal(Ogre::SceneManager *sceneManager){
  _sheet = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  _sceneManager = sceneManager;
}

ScenePlayFinal::~ScenePlayFinal(){}

CEGUI::Window *ScenePlayFinal::getSheet(){
	return _sheet;
}

void ScenePlayFinal::crearCeguiFinal(){
	CEGUI::Window* vent = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("PuntuacionHunter.layout");
	vent->setPosition(CEGUI::UVector2(CEGUI::UDim(0.13f,0),CEGUI::UDim(0.16f,0)));
	_sheet -> addChild(vent);

	CEGUI::Window* exitButton = vent->getChild("ExitButton");
  	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&FinalState::exitButtonC, 
                FinalState::getSingletonPtr())); 

}

void ScenePlayFinal::PointsAndPower(){

	CEGUI::Window* vent = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("PointsHunter.layout");
	vent->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65f,0),CEGUI::UDim(0.00f,0)));
	_sheet -> addChild(vent);

	CEGUI::Window* vent1 = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("PowerHunter.layout");
	vent1->setPosition(CEGUI::UVector2(CEGUI::UDim(0.50f,0),CEGUI::UDim(0.00f,0)));
	_sheet -> addChild(vent1);


}