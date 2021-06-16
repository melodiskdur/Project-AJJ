#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{

}

//Getters
Camera* Scene::getCamera()
{
	return scene_camera;
}

std::vector<sf::Sprite*> Scene::getObjectSprites()
{
	std::vector<sf::Sprite*> objectSprites;
	for (Object* o : scene_objects)
	{
		//Retreive sprite from each object in the scene
	}
}

//Setters
void Scene::setCamera(Camera* camera)
{
	scene_camera = camera;
}

//Etc
void Scene::addSceneObject(Object* object)
{
	scene_objects.push_back(object);
}

void Scene::addSceneObjects(std::vector<Object*> objects)
{
	scene_objects.insert(scene_objects.end(), objects.begin(), objects.end());
}
