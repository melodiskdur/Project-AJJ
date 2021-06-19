#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Scene.h"

/* ExtendedRenderWindow
	Inherits from sf::RenderWindow. Contains additional functionality
	to be able to handle Scene instances directly. The purpose of the class
	is to be able to easily draw whole Scenes rather than individual Objects.
*/
class ExtendedRenderWindow : public sf::RenderWindow
{
public:
	ExtendedRenderWindow(sf::Vector2u resolution, std::string window_title);
	~ExtendedRenderWindow();

	//Getters
	Scene* getActiveScene();
	
	//Setters
	void setActiveScene(Scene* scene);
	
	//Etc
	//Draws a frame of the active Scene-object.
	void drawActiveScene();
private:
	Scene* active_scene = nullptr;			//Active Scene-object.
};

