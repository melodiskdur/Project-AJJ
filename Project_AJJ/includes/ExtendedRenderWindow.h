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
	ExtendedRenderWindow(sf::Vector2u resolution, std::string window_title, sf::ContextSettings settings);
	~ExtendedRenderWindow();

	//Getters
	Scene* getActiveScene();
	TextureManager* getTextureManager();
	bool getWindowState() { return this->window_state; }
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
	void drawLayoutObject(Object * obj, sf::RenderTexture* render_texture);
	//draw an object on the screen
	void drawObject(Object* obj);
	//draw the layouts and all of the layouts contained inside
	void drawLayouts(Layout* layout, sf::RenderTexture* render_texture);
	//draw a specific layout
	void drawLayout(Layout* layout, sf::RenderTexture* render_texture);
	//debug view for the layout. Draws the layout and the contained margin_spaces
	void drawLayoutsDEBUG(Layout* layout);

	//deactivates the window
	void deactivateWindow() { this->window_state = false; }
	//activates the window
	void activateWindow() { this->window_state = true; }
	
	static unsigned int instanceCount() { return instance_counter; };

private:
	Scene* active_scene = nullptr;							//Active Scene-object.
	std::vector<Scene*> scenes;								//All of the available scenes
	TextureManager* texture_manager = nullptr;				//TextureManager-object.
	std::vector<sf::RenderTexture*> scene_layer_textures;	//Off-screen rendering of the active scene's layers.
	bool window_state = true;								//Activated/deactivated

	//Used internally to clear all RenderTextures in preparation for the next frame draw.
	void clearSceneLayerTextures();
	//Draws layers in descending order (layer_num). Used internally in drawActiveScene().
	void drawLayers();

	// Debugging.
	bool debugger_mode = true;
	void debugDraw();
	// End debugging.

	static unsigned int instance_counter;
};

