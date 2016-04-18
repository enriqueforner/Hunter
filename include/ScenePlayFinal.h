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
	void PointsAndPower();
	CEGUI::Window* getSheet();
	CEGUI::Window* getPointsWindow();
	CEGUI::Window* getPowerWindow();
	void updatePower(int power);
	void updatePoints(int points);
	void cleanCegui();
private:

	Ogre::SceneManager* _sceneManager;
	CEGUI::Window* _sheet;
	CEGUI::Window* _pointsWindow;
	CEGUI::Window* _powerWindow;
};
#endif