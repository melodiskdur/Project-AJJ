#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TextureAtlas.h"
#include "TextureIds.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Layout.h"
#include "Button.h"

/* ExtendedRenderWindow
	Inherits from sf::RenderWindow. Contains additional functionality
	to be able to handle Scene instances directly. The purpose of the class
	is to be able to easily draw whole Scenes rather than individual Objects.
*/
class ExtendedRenderWindow : public sf::RenderWindow
{
public:
	ExtendedRenderWindow(sf::Vector2u resolution, std::string window_title, sf::ContextSettings settings);
	~ExtendedRenderWindow();

	//Getters
	Scene* getActiveScene();
	TextureManager* getTextureManager();
	bool getWindowState() { return this->window_state; }
	std::vector<Layout*> getLayouts() { return this->layouts; }
	std::vector<Scene*> getScenes() { return this->scenes; }
	Scene* getSceneFromDenotation(SCENE_DENOTATION scene_denotation);

	//Setters
	//Sets active scene and creates RenderTexture-pointers for each SceneLayer in active_scene.
	void setActiveScene(Scene* scene);
	void setTextureManager(TextureManager* tex_mag);
	
	//Etc
	//Draws a frame of the active Scene-object.
	void drawActiveScene();
	void deactivateWindow() { this->window_state = false; }
	void activateWindow() { this->window_state = true; }
	void drawButton(Button * button);
	void drawLayouts(Layout* parent_layout);
	void drawLayout(Layout* layout);
	void drawLayoutsDEBUG(Layout* layout);
	void addLayout(Layout* new_layout) { this->layouts.push_back(new_layout); }
	
private:
	Scene* active_scene = nullptr;							//Active Scene-object.
	std::vector<Scene*> scenes;								//All of the available scenes
	TextureManager* texture_manager = nullptr;				//TextureManager-object.
	std::vector<sf::RenderTexture*> scene_layer_textures;	//Off-screen rendering of the active scene's layers.
	bool window_state = true;								//Activated/deactivated
	std::vector<Layout*> layouts;							//Windows layouts(main_menu,side_menus,messages,etc.)

	//Used internally to clear all RenderTextures in preparation for the next frame draw.
	void clearSceneLayerTextures();
	//Draws layers in descending order (layer_num). Used internally in drawActiveScene().
	void drawLayers();

	// Debugging.
	bool debugger_mode = false;
	// Debugging.
	void debugDraw();
	// End debugging.
};

