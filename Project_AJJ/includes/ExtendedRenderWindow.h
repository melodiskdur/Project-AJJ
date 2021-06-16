#pragma once

#include <SFML/Graphics.hpp/>
#include "Scene.h"
/*

*/
class ExtendedRenderWindow : public sf::RenderWindow
{
public:
	ExtendedRenderWindow();
	~ExtendedRenderWindow();

	//Getters
	//Setters
	//Etc
	void displayActiveScene();
private:
	Scene* activeScene;
};

