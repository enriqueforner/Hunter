//IntroScene.h

#ifndef IntroScene_H
#define IntroScene_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>

class IntroScene {
public:

	IntroScene(Ogre::SceneManager *sceneManager);
	~IntroScene();
	void crearWorld();
	void crearMenuInicioCEGUI();
	void destroyCegui();
	void creditos();


private:

	Ogre::SceneManager* _sceneManager;
	CEGUI::Window* _sheet;
	//bool *_play;
	
};
#endif