//ScenePlayFinal.cpp
#include "ScenePlayFinal.h"
#include "FinalState.h"
#include <sstream>

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

	_pointsWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("PointsHunter.layout");
	_pointsWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65f,0),CEGUI::UDim(0.00f,0)));
	_sheet -> addChild(_pointsWindow);
	_pointsWindow->enable();
	_pointsWindow->setVisible(true);

	_powerWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("PowerHunter.layout");
	_powerWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.50f,0),CEGUI::UDim(0.00f,0)));
	_sheet -> addChild(_powerWindow);
	_powerWindow->enable();
	_powerWindow->setVisible(true);



}

CEGUI::Window *ScenePlayFinal::getPointsWindow(){
	return _pointsWindow;
}

CEGUI::Window *ScenePlayFinal::getPowerWindow(){
	return _powerWindow;
}

void ScenePlayFinal::updatePower(int power){
    //CEGUI::Window* vent = _sheet -> getChild("PowerWindow");
    // int points_actuales = atoi(points->getText().c_str());
    // points_actuales = newpuntos + points_actuales;
    std::ostringstream os;
    os << power;
    _powerWindow ->setText(os.str());
    _powerWindow ->enable();
    _powerWindow ->render();
    std::cout << "TEXTO POWER "<<_powerWindow->getText() << std::endl;
}

void ScenePlayFinal::updatePoints(int points){
    //CEGUI::Window* vent = _sheet -> getChild("PointsWindow");
    // int points_actuales = atoi(points->getText().c_str());
    // points_actuales = newpuntos + points_actuales;
    std::ostringstream os;
    os << points;
    _pointsWindow -> setText(os.str());
    _pointsWindow ->enable();
    _pointsWindow -> render();
    std::cout << "TEXTO POINTS "<<_pointsWindow->getText() << std::endl;
}