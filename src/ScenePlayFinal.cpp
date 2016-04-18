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

    CEGUI::Window* mmbutton = vent->getChild("MainMenuButton");
      mmbutton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&FinalState::mainmenuButtonC, 
                FinalState::getSingletonPtr())); 

    //CEGUI::Window* puntosplayer = _sheet->getChild("Puntuacion");
    vent->getChild("PuntosPlayer")->setText(_sheet->getChild("PointsWindow")->getChild("StaticText")->getText());
}

void ScenePlayFinal::PointsAndPower(){

	_pointsWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("PointsHunter.layout");
	_pointsWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.65f,0),CEGUI::UDim(0.00f,0)));
	_sheet -> addChild(_pointsWindow);
	_pointsWindow->enable();
	_pointsWindow->setVisible(true);
	_pointsWindow->setAlwaysOnTop(true);
	//_pointsWindow->setClippedByParent(false);
	_pointsWindow->activate();

	//std::cout << "POINTS RECIEN ACTIVA" <<_pointsWindow->isActive() << std::endl;

	_powerWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("PowerHunter.layout");
	_powerWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0.50f,0),CEGUI::UDim(0.00f,0)));
	_sheet -> addChild(_powerWindow);
	_powerWindow->enable();
	_powerWindow->setVisible(true);
	_powerWindow->setAlwaysOnTop(true);
	//_powerWindow->setClippedByParent(false);
	_powerWindow->activate();
/*
	std::cout << "SHEET ACTIVA" <<_sheet->isActive() << std::endl;
	std::cout << "POINTS ACTIVA" <<_pointsWindow->isActive() << std::endl;
	std::cout << "POWER ACTIVA" <<_powerWindow->isActive() << std::endl;

	std::cout << "POINTS CLIPPED" <<_pointsWindow->isClippedByParent() << std::endl;
	std::cout << "POWER CLIPPED" <<_powerWindow->isClippedByParent() << std::endl;
*/
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
    os << power <<"%";
    _powerWindow->activate();
    _powerWindow->getChild("StaticText")->moveToFront();
    _powerWindow ->setText(os.str());
    _powerWindow ->enable();
    _powerWindow ->render();
    //std::cout << "TEXTO POWER "<<_powerWindow->getText() << std::endl;

    _powerWindow->getChild("StaticText")->moveToFront();
    _powerWindow->getChild("StaticText")->activate();
    _powerWindow->getChild("StaticText")->enable();
    _powerWindow->getChild("StaticText")->render();
    _powerWindow->getChild("StaticText")->setText(os.str());

    //std::cout << "TEXTO POWER STATIC "<<_powerWindow->getChild("StaticText")->getText() << std::endl;
}

void ScenePlayFinal::updatePoints(int points){
    //CEGUI::Window* vent = _sheet -> getChild("PointsWindow");
    // int points_actuales = atoi(points->getText().c_str());
    // points_actuales = newpuntos + points_actuales;
    std::ostringstream os;
    os << points;
    _pointsWindow->activate();
    _pointsWindow->getChild("StaticText")->moveToFront();
    _pointsWindow -> setText(os.str());
    _pointsWindow ->enable();
    _pointsWindow -> render();
    //std::cout << "TEXTO POINTS "<<_pointsWindow->getText() << std::endl;

    _pointsWindow->getChild("StaticText")->moveToFront();
    _pointsWindow->getChild("StaticText")->activate();
    _pointsWindow->getChild("StaticText")->enable();
    _pointsWindow->getChild("StaticText")->render();
    _pointsWindow->getChild("StaticText")->setText(os.str());

    //std::cout << "TEXTO POINTS STATIC "<<_powerWindow->getChild("StaticText")->getText() << std::endl;
}

void ScenePlayFinal::cleanCegui(){
    _sheet-> destroyChild("PowerWindow");
    _sheet-> destroyChild("PointsWindow");
    _sheet-> destroyChild("Puntuacion");

}