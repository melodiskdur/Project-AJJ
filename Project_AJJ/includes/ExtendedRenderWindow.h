#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureAtlas.h"
#include "TextureIds.h"
#include "TextureManager.h"
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
	TextureManager* getTextureManager();
	
	//Setters
	void setActiveScene(Scene* scene);
	void setTextureManager(TextureManager* tex_mag);
	
	//Etc
	//Draws a frame of the active Scene-object.
	void drawActiveScene();
private:
	Scene* active_scene = nullptr;			    //Active Scene-object.
	TextureManager* texture_manager = nullptr;	//TextureManager-object.
};

