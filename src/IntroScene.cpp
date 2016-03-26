
#include "IntroScene.h"
#include "IntroState.h"

IntroScene::IntroScene(Ogre::SceneManager* sceneManager){
  _sheet = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  _sceneManager = sceneManager;
}
IntroScene::~IntroScene(){

  
}

void IntroScene::crearMenuInicioCEGUI(){

	CEGUI::Window* vent = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("hunterMP.layout");
	vent->setPosition(CEGUI::UVector2(CEGUI::UDim(0.22f,0),CEGUI::UDim(0.18f,0)));
	_sheet -> addChild(vent);
	vent-> moveToFront();

	//BOTONES
	CEGUI::Window* exitButton = vent->getChild("ExitButton");
  	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&IntroState::exitButtonC, 
                IntroState::getSingletonPtr())); 

}

void IntroScene::crearWorld(){

	Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
  	Ogre::MeshManager::getSingleton().createPlane("planeIni",
  	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
  	1024,768,1,1,true,1,20,20,Ogre::Vector3::UNIT_Z);

  	Ogre::SceneNode* node2 = _sceneManager->createSceneNode("ground");
  	Ogre::Entity* groundEnt = _sceneManager->createEntity("planeEntIni", "planeIni");
  	groundEnt->setMaterialName("Ground");
  	node2->attachObject(groundEnt);

  	_sceneManager->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);	
  	Ogre::Light* light = _sceneManager->createLight("LightIni");
  	light->setType(Ogre::Light::LT_DIRECTIONAL);
  	light->setDirection(Ogre::Vector3(1,-1,0));
  	node2->attachObject(light);

  	_sceneManager->getRootSceneNode()->addChild(node2);

  	Ogre::Entity* ent1 = _sceneManager->createEntity("CerdoInicio", "CerdoIni.mesh");
  	Ogre::SceneNode* nodecer = _sceneManager->createSceneNode("CerdoIni");
  	nodecer->attachObject(ent1);
  	_sceneManager->getRootSceneNode()->addChild(nodecer);

}