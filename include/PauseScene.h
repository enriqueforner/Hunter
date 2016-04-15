
#ifndef PauseScene_H
#define PauseScene_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>

class PauseScene {
public:

	PauseScene(Ogre::SceneManager *sceneManager);
	~PauseScene();
	void crearCegui();
	bool destroyCegui();
	void limpiarpantallaCEGUI();
	void menuCegui();

private:

	Ogre::SceneManager* _sceneManager;
	CEGUI::Window* _sheet;
	
	
};
#endif