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

std::vector<SceneLayer*>& Scene::getSceneLayers()
{
	return this->scene_layers;
}

//Setters
void Scene::setCamera(Camera* camera)
{
	scene_camera = camera;
	for (int i = 0; i < this->scene_layers.size(); i++)
	{
		this->scene_layers[i]->last_cam_pos = this->scene_camera->getCameraPosition();
	}
}

void Scene::setCollisionDetection(CollisionDetection* col)
{
	col_det = col;
}

//Etc
void Scene::createSceneLayer(signed int layer_num, float depth, float scale)
{
	SceneLayer* new_layer = new SceneLayer();
	new_layer->layer_num = layer_num;
	new_layer->depth = depth;
	new_layer->scale = scale;
	if (!this->addLayer(new_layer))
		delete new_layer;
}

void Scene::addSceneObject(Object* object)
{
	scene_objects.push_back(object);
}

void Scene::addSceneObjects(std::vector<Object*> objects)
{
	scene_objects.insert(scene_objects.end(), objects.begin(), objects.end());
}

void Scene::addObjectToSceneLayer(Object* object, int layer_num)
{
	//Finds the SceneLayer with the corresponding layer_num in vector and adds object to it.
	for (int i = 0; i < this->scene_layers.size(); i++)
	{
		if (scene_layers[i]->layer_num == layer_num)
		{
			scene_layers[i]->layer_objects.push_back(object);
			break;
		}
	}
}

void Scene::addObjectsToSceneLayer(std::vector<Object*> objects, int layer_num)
{
	//Fins the SceneLayer with the corresponding layer_num in vector and inserts the object vector into it.
	for (int i = 0; i < this->scene_layers.size(); i++)
	{
		if (scene_layers[i]->layer_num == layer_num)
		{
			scene_layers[i]->layer_objects.insert(scene_objects.end(), objects.begin(), objects.end());
			break;
		}
	}
}

void Scene::updateSceneFrame()
{
	col_det->checkForCollisions(scene_camera->getCameraViewRect());
}

void Scene::updateSceneLayers()
{
	sf::Vector2f offset;
	for (int i = 0; i < this->scene_layers.size(); i++)
	{
		SceneLayer* current = this->scene_layers[i];					 //For readability.
		offset = this->calculateLayerOffset(current);					 //Get position offset.
		current->last_cam_pos = this->scene_camera->getCameraPosition();//Update layer cam info.
		//Reduces the world position of each object in every layer by the calculated offset vector.
		for (Object* o : current->layer_objects)
		{
			o->setWorldPosition(o->getWorldPosition() - offset);
		}
	}
}

bool Scene::addLayer(SceneLayer* layer)
{
	for (int i = 0; i < this->scene_layers.size(); i++)
	{
		//Vector can't contain layer_num duplicates.
		if (this->scene_layers[i]->layer_num == layer->layer_num)
			return false;
		//If an element has a lower layer_num than layer (param), layer is inserted right before that element.
		else if (this->scene_layers[i]->layer_num < layer->layer_num)
		{
			this->scene_layers.insert(scene_layers.begin() + i, layer);
			return true;
		}
	}
	//Runs if scene_layers is empty or if layer has the smallest layer_num, thus adding it to the back of the vector.
	this->scene_layers.push_back(layer);
	return true;
}

sf::Vector2f Scene::calculateLayerOffset(SceneLayer* layer)
{
	//Depth value can't be 0.
	if (layer->depth == 0)
		return sf::Vector2f(0.0f, 0.0f);

	float offset_factor;
	sf::Vector2f current_cam_pos = this->scene_camera->getCameraPosition();
	//Vector difference between a layer's last calculated cam pos and the current cam pos.
	sf::Vector2f diff = layer->last_cam_pos - current_cam_pos;
	//If vector difference is 0 (handling them as floats).
	if (std::abs(diff.x) < 0.01 && std::abs(diff.y) < 0.01)
		return sf::Vector2f(0.0f, 0.0f);

	//Offset factor is calculated and multiplied with diff vector.
	offset_factor = 1.0f - (1.0f / layer->depth);
	return diff * offset_factor;
}