#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Camera.h"

/*
	Class Scene
*/
class Scene
{
public:
	Scene();
	~Scene();

	//Getters
	Camera* getCamera();
	std::vector<sf::Sprite*> getObjectSprites();
	//Setters
	void setCamera(Camera* camera);
	//Etc
	void addSceneObject(Object* object);
	void addSceneObjects(std::vector<Object*> objects);
private:
	Camera* scene_camera;
	std::vector<Object*> scene_objects;
};