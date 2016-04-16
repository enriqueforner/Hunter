
#include "IntroScene.h"
#include "IntroState.h"
#include "Ranking.h"

IntroScene::IntroScene(Ogre::SceneManager* sceneManager){
  _sheet = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
  _sceneManager = sceneManager;
}
IntroScene::~IntroScene(){

  
}

void IntroScene::crearMenuInicioCEGUI(){

	CEGUI::Window* vent = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("hunterMP.layout");
  CEGUI::Window* ventCreditos = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("creditos.layout");
  CEGUI::Window* ventranking = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("rankingl.layout");

  ventCreditos -> setVisible(false);
  ventranking -> setVisible(false);

  vent->setPosition(CEGUI::UVector2(CEGUI::UDim(0.22f,0),CEGUI::UDim(0.18f,0)));
	ventCreditos->setPosition(CEGUI::UVector2(CEGUI::UDim(0.18f,0),CEGUI::UDim(0.18f,0)));
  ventranking->setPosition(CEGUI::UVector2(CEGUI::UDim(0.18f,0),CEGUI::UDim(0.01f,0)));
  
  _sheet -> addChild(vent);
  _sheet -> addChild(ventCreditos);
  _sheet -> addChild(ventranking);

	vent-> moveToFront();

	//BOTONES MP
	CEGUI::Window* exitButton = vent->getChild("ExitButton");
  	exitButton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&IntroState::exitButtonC, 
                IntroState::getSingletonPtr())); 
  CEGUI::Window* playButton = vent->getChild("PlayButton");
    playButton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&IntroState::playButtonC, 
                IntroState::getSingletonPtr())); 
  CEGUI::Window* creditsButton = vent->getChild("CreditsButton");
    creditsButton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&IntroState::creditsButtonC, 
                IntroState::getSingletonPtr()));
  CEGUI::Window* rankingButton = vent->getChild("RankingButton");
    rankingButton ->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&IntroState::rankingButtonC, 
                IntroState::getSingletonPtr()));  

  //BOTONES CREDITS  
  CEGUI::Window* atrasCButton = ventCreditos->getChild("AtrasCredits");
    atrasCButton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&IntroState::retroceder_c, 
                IntroState::getSingletonPtr()));
  //BOTONES Ranking  
  CEGUI::Window* atrasRButton = ventranking->getChild("AtrasRanking");
    atrasRButton->subscribeEvent(CEGUI::PushButton::EventClicked,
          CEGUI::Event::Subscriber(&IntroState::retroceder_r, 
                IntroState::getSingletonPtr()));
  //Imagenes RANKING
  CEGUI::Window* imgnameranking = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","InameRanking");
  CEGUI::ImageManager::getSingleton().addFromImageFile("InameRanking","NameMPH.png");
  imgnameranking -> setProperty("Image","InameRanking");
  imgnameranking -> setProperty("BackgroundEnabled","False");
  imgnameranking -> setProperty("FrameEnabled","False");
  imgnameranking -> setSize(CEGUI::USize(CEGUI::UDim(0.10f,0),CEGUI::UDim(0.14f,0)));  
  imgnameranking -> setPosition(CEGUI::UVector2(CEGUI::UDim(0.21f,0),CEGUI::UDim(0.12f,0)));
   ventranking -> addChild(imgnameranking);  

  CEGUI::Window* imgnamepoints = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","IpointsRanking");
  CEGUI::ImageManager::getSingleton().addFromImageFile("IpointsRanking","PointsMPH.png");
  imgnamepoints -> setProperty("Image","IpointsRanking");
  imgnamepoints -> setProperty("BackgroundEnabled","False");
  imgnamepoints -> setProperty("FrameEnabled","False");
  imgnamepoints -> setSize(CEGUI::USize(CEGUI::UDim(0.10f,0),CEGUI::UDim(0.14f,0)));  
  imgnamepoints -> setPosition(CEGUI::UVector2(CEGUI::UDim(0.36f,0),CEGUI::UDim(0.12f,0)));
  ventranking -> addChild(imgnamepoints); 

  CEGUI::Window* imgnamehunter = CEGUI::WindowManager::getSingleton().createWindow("TaharezLook/StaticImage","INameHunter");
  CEGUI::ImageManager::getSingleton().addFromImageFile("INameHunter","huntername.png");
  imgnamehunter -> setProperty("Image","INameHunter");
  imgnamehunter -> setProperty("BackgroundEnabled","False");
  imgnamehunter -> setProperty("FrameEnabled","False");
  imgnamehunter -> setSize(CEGUI::USize(CEGUI::UDim(0.23f,0),CEGUI::UDim(0.34f,0)));  
  imgnamehunter -> setPosition(CEGUI::UVector2(CEGUI::UDim(0.12f,0),CEGUI::UDim(0.76f,0)));
  _sheet -> addChild(imgnamehunter);     

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

  	Ogre::Entity* entc = _sceneManager->createEntity("CerdoIni", "CerdoIni.mesh");
  	Ogre::SceneNode* nodecer = _sceneManager->createSceneNode("CerdoIni");
  	nodecer->attachObject(entc);
    nodecer-> setPosition(9,5,20);
    nodecer->setScale(3,3,3);
    // roll --> z
    // pitch --> x
    // yaw --> y
    nodecer->yaw(Ogre::Degree(35));
    _sceneManager->getRootSceneNode()->addChild(nodecer);

    Ogre::Entity* entl = _sceneManager->createEntity("LoboInicio", "Lobo.mesh");
    Ogre::SceneNode* nodelob = _sceneManager->createSceneNode("LoboIni");
    nodelob->attachObject(entl);
    nodelob-> setPosition(-34,5,-30);
    nodelob->setScale(5,5,5);
    nodelob->yaw(Ogre::Degree(150));
    _sceneManager->getRootSceneNode()->addChild(nodelob);


}

void IntroScene::creditos(){
  CEGUI::Window* vent = _sheet-> getChild("hunterMenuP");
  vent->setVisible(false);
  CEGUI::Window* ventCreditos = _sheet-> getChild("Creditos");
  ventCreditos -> setVisible(true);

}
void IntroScene::ranking(){
  CEGUI::Window* vent = _sheet-> getChild("hunterMenuP");
  vent->setVisible(false);
  CEGUI::Window* ventranking = _sheet-> getChild("RankingL");
  ventranking -> setVisible(true);

}

void IntroScene::destroyCegui(){
  _sheet-> destroyChild("hunterMenuP");
  _sheet-> destroyChild("Creditos");
  _sheet-> getChild("RankingL")->destroyChild("InameRanking");
  CEGUI::ImageManager::getSingleton().destroy("InameRanking");
  _sheet-> getChild("RankingL")->destroyChild("IpointsRanking");
  CEGUI::ImageManager::getSingleton().destroy("IpointsRanking");
  _sheet-> destroyChild("RankingL");
  _sheet-> destroyChild("INameHunter");
  CEGUI::ImageManager::getSingleton().destroy("INameHunter");

}

void IntroScene::limpiarpantallaCEGUI(){
  CEGUI::Window* vent = _sheet-> getChild("hunterMenuP");
  CEGUI::Window* ventcredits = _sheet-> getChild("Creditos");
  CEGUI::Window* ventranking = _sheet-> getChild("RankingL");
  if(vent-> isVisible()){
    vent->setVisible(false);
  }
  if(ventcredits-> isVisible()){
    ventcredits->setVisible(false);
  }
  if(ventranking-> isVisible()){
    ventranking->setVisible(false);
  }
  
}

void IntroScene::retroceder(){
  CEGUI::Window* ventCreditos = _sheet-> getChild("Creditos");
  CEGUI::Window* ventranking = _sheet-> getChild("RankingL");
  
  if(ventCreditos->isVisible()){
    ventCreditos -> setVisible(false);
  }
  if(ventranking->isVisible()){
    ventranking -> setVisible(false);
  }
  CEGUI::Window* vent = _sheet-> getChild("hunterMenuP");
  vent->setVisible(true);

}

void IntroScene::ActRanking(){
  Ranking* ran = new Ranking();
  ran -> leerPlayeryPuntos();
  ran -> acturanking();
}