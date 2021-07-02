#include "Scene.h"

Scene::Scene()
{
}

Scene::~Scene()
{
	for (Object* o : scene_objects)
		delete o;
	std::cout << "Scene deleted" << std::endl;
}

//Getters
Camera* Scene::getCamera()
{
	return scene_camera;
}

/*
std::vector<sf::Sprite*> Scene::getObjectSprites()
{
	std::vector<sf::Sprite*> objectSprites;
	for (Object* o : scene_objects)
	{
		//Retreive sprite from each object in the scene
	}
}
*/

std::vector<Object*>& Scene::getSceneObjects()
{
	return scene_objects;
}


//Setters
void Scene::setCamera(Camera* camera)
{
	scene_camera = camera;
}

void Scene::setCollisionDetection(CollisionDetection* col)
{
	col_det = col;
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

void Scene::updateSceneFrame()
{
	col_det->checkForCollisions(scene_camera->getCameraViewRect());
}