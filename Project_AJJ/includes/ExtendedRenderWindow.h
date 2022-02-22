#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
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
	
	/*Etc*/
	//Draws a frame of the active Scene-object.
	void drawActiveScene();
	//draw a button on the screen
	void drawLayoutObject(Object * obj);
	//draw an object on the screen
	void drawObject(Object* obj);
	//draw the layouts and all of the layouts contained inside
	void drawLayouts(Layout* parent_layout);
	//draw a specific layout
	void drawLayout(Layout* layout);
	//debug view for the layout. Draws the layout and the contained margin_spaces
	void drawLayoutsDEBUG(Layout* layout);

	//adds a layout to the vector of layouts in the window
	void addLayout(Layout* new_layout) { this->layouts.push_back(new_layout); }

	//deactivates the window
	void deactivateWindow() { this->window_state = false; }
	//activates the window
	void activateWindow() { this->window_state = true; }

	//returns true or false if the cursor is over the rect
	bool cursorHover(sf::FloatRect rect);
	//returns a vector of rects that the cursor is hovering from a set of rects
	std::vector<sf::FloatRect> cursorHoverRects(std::vector<sf::FloatRect> rects);
	
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
	void debugDraw();
	// End debugging.
};

