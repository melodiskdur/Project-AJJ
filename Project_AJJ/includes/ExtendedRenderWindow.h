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
	//Sets active scene and creates RenderTexture-pointers for each SceneLayer in active_scene.
	void setActiveScene(Scene* scene);
	void setTextureManager(TextureManager* tex_mag);
	
	//Etc
	//Draws a frame of the active Scene-object.
	void drawActiveScene();
private:
	Scene* active_scene = nullptr;			    //Active Scene-object.
	TextureManager* texture_manager = nullptr;	//TextureManager-object.
	std::vector<sf::RenderTexture*> scene_layer_textures;

	//Used internally to clear all RenderTextures in preparation for the next frame draw.
	void clearSceneLayerTextures();
	//Draws layers in descending order (layer_num). Used internally in drawActiveScene().
	void drawLayers();
};

