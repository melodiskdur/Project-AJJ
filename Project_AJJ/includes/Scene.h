#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Camera.h"
#include "PhysicsAttribute.h"
#include "CollisionDetection.h"

class CollisionDetection;

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
	
	//Setters
	void setCamera(Camera* camera);
	void setCollisionDetection(CollisionDetection* col);
	
	//
	//Adds object(s) 
	void addSceneObject(Object* object);
	void addSceneObjects(std::vector<Object*> objects);
	//Updates the scene (object positions etc) for the drawing of the next frame.
	void updateSceneFrame();
private:
	Camera* scene_camera = nullptr;
	std::vector<Object*> scene_objects;
	CollisionDetection* col_det = nullptr;
};