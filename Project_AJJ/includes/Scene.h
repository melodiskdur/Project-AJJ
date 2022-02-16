#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Camera.h"
#include "PhysicsManager.h"
#include "PhysicsAttribute.h"
#include "CollisionDetection.h"

class CollisionDetection;
class PhysicsManager;

/* SceneLayer
*  Struct that contains a vector of objects as well as some parameters to calculate how
*  far away the layer is (depth), the scaling of the layer (scale) as well as the previous
*  position of the scene camera. A larger depth corresponds to a slower parallax. A depth of 1
*  yields no parallax at all. Depths in range ]0, 1[ result in a reverse parallax, useful if the
*  layer is in the foreground of the game scene layer.
 */
typedef struct _SceneLayer
{
	std::vector<Object*> layer_objects;
	float depth;
	float scale;
    signed int layer_num;	    //Rendering order. x = 0: interactive scene layer. x > 0 background, x < 0 : foreground.
	sf::Vector2f last_cam_pos;
} SceneLayer;

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
	//std::vector<sf::Sprite*> getObjectSprites();
	std::vector<Object*>& getSceneObjects();
	std::vector<SceneLayer*>& getSceneLayers();
	Object* getObjectWithId(int id);
	CollisionDetection* getCollisionDetection();
	PhysicsManager* getPhysicsManager() { return this->phys_mag; };
	SCENE_DENOTATION getSceneDenotation() { return this->scene_denotation; }

	//Setters
	void setCamera(Camera* camera);
	void setCollisionDetection(CollisionDetection* col);
	void setPhysicsManager(PhysicsManager* phys);
	SCENE_DENOTATION setSceneDenotation( SCENE_DENOTATION new_denotation ) { this->scene_denotation = new_denotation; }
	
	//Others
	// Instantiates a new SceneLayer given that the layer_num has not already been occupied.
	void createSceneLayer(signed int layer_num, float depth, float scale);
	//Adds object(s) to scene (the interactive part e.g players, tiles, mobs).
	void addSceneObject(Object* object);
	void addSceneObjects(std::vector<Object*> objects);
	//Adds object(s) to a SceneLayer with corresponding SceneLayer::layer_num.
	void addObjectToSceneLayer(Object* object, int layer_num);
	void addObjectsToSceneLayer(std::vector<Object*> objects, int layer_num);
	//Updates the scene (object positions etc) for the drawing of the next frame.
	void updateSceneFrame();
	//Updates scene layers.
	void updateSceneLayers();


private:
	Camera* scene_camera = nullptr;											//the scenes camera
	std::vector<Object*> scene_objects;										//the objects currently in the scene
	SCENE_DENOTATION scene_denotation = SCENE_DENOTATION::TEST_GAME;		//denotation of the scene i.e. name/class/defintion
	CollisionDetection* col_det = nullptr;									//the scenes collisiondetection-handler
	PhysicsManager* phys_mag = nullptr;										//the scenes physics-manager
	std::vector<SceneLayer*> scene_layers;									//all of the layers present in the scene

	//Adds a layer into vector scene_layers and sorts it in descending order (layer.layer_num). Returns
	//false if there already is a layer with the same layer_num.
	bool addLayer(SceneLayer* layer);

	//Sets the SceneLayer offset with respect to the Scene camera's movement and position. Called in Scene::updateSceneLayers.
	sf::Vector2f calculateLayerOffset(SceneLayer* layer);
};