#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Camera.h"
#include "SceneLayer.h"
#include "PhysicsManager.h"
#include "PhysicsAttribute.h"
#include "CollisionDetection.h"

class CollisionDetection;
class PhysicsManager;
class SceneLayer;
class Layout;

//denotation of the scene i.e. name/class/defintion
enum SCENE_DENOTATION
{
	TEST_GAME,
	MAIN_MENU,
	ABOUT_US,
	OPTIONS,
	MAP_EDITOR,
	CUSTOM
};

/* Scene
	Creates an environment in which many Objects, as well as a Camera object, can be placed.
	The Scene can then be rendered onto a surface (ExtendedRenderWindow is recommended for this
	as it can handle Scene instances directly) from the Camera viewpoint. The idea is to keep
	the Scene as generalized as possible, creating opportunity to derive many different Scene types
	(maps, menu screens, cutscenes etc).
*/

class Scene
{
public:
	Scene();
	~Scene();

	//Getters
	Camera* getCamera();
	std::vector<Object*>& getSceneObjects() { return this->scene_objects; };
	// Sorts through the Objects of a SceneLayer corresponding to the layer_num parameter.
	std::vector<Object*> getObjectsWithinCamera(signed int layer_num);
	std::vector<Layout*> getLayoutsWithinCamera(signed int layer_num);
	std::vector<SceneLayer*>& getSceneLayers() { return this->scene_layers;};
	SceneLayer* getLayer(int layer_num);
	Object* getObjectWithId(int id);
	CollisionDetection* getCollisionDetection();
	PhysicsManager* getPhysicsManager() { return this->phys_mag; };
	SCENE_DENOTATION getSceneDenotation() { return this->scene_denotation; };
	std::vector<int> getLayerNums() { return this->layer_nums; };

	//Setters
	void setCamera(Camera* camera);
	void setCollisionDetection(CollisionDetection* col);
	void setPhysicsManager(PhysicsManager* phys);
	SCENE_DENOTATION setSceneDenotation( SCENE_DENOTATION new_denotation ) { this->scene_denotation = new_denotation; }
	
	//Others
	// DEBUGGING.
	void addSceneLayer(SceneLayer* L);
	// END DEBUGGING.
	//Adds object(s) to scene (the interactive part e.g players, tiles, mobs).
	void addSceneObject(Object* object);
	void addSceneObjects(std::vector<Object*> objects);
	//Adds object(s) to a SceneLayer with corresponding SceneLayer::layer_num.
	void addObjectToSceneLayer(Object* object, int layer_num);
	void addObjectsToSceneLayer(std::vector<Object*> objects, int layer_num);
	//Updates the scene (object positions etc) for the drawing of the next frame.
	void updateSceneFrame();
	std::vector<sf::View> getLayerManipulatedViews();
	std::map<int, sf::View> getLayerManipulatedView();

	static unsigned int instanceCount() { return instance_counter; };
private:
	Camera* scene_camera = nullptr;											//the scenes camera
	std::vector<Object*> scene_objects;										//the objects currently in the scene
	SCENE_DENOTATION scene_denotation = SCENE_DENOTATION::TEST_GAME;		//denotation of the scene i.e. name/class/defintion
	CollisionDetection* col_det = nullptr;									//the scenes collisiondetection-handler
	PhysicsManager* phys_mag = nullptr;										//the scenes physics-manager
	std::vector<SceneLayer*> scene_layers;									//all of the layers present in the scene
	std::vector<int> layer_nums;

	//Adds a layer into vector scene_layers and sorts it in descending order (layer.layer_num). Returns
	//false if there already is a layer with the same layer_num.
	bool addLayer(SceneLayer* layer);

	static unsigned int instance_counter;
};