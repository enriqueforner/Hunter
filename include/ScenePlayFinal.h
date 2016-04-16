#ifndef ScenePlayFinal_H
#define ScenePlayFinal_H

#include <Ogre.h>
#include <OIS/OIS.h>
#include <CEGUI.h>
#include <RendererModules/Ogre/Renderer.h>

class ScenePlayFinal {
public:

	ScenePlayFinal(Ogre::SceneManager *sceneManager);
	~ScenePlayFinal();
	void crearCeguiFinal();
private:

	Ogre::SceneManager* _sceneManager;
	CEGUI::Window* _sheet;
	
	
};
#endif