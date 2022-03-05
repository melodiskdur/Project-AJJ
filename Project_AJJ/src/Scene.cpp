#include "Scene.h"

unsigned int Scene::instance_counter = 0;

Scene::Scene()
{
	Scene::instance_counter++;
	// Create the main Scene-layer.
	this->addSceneLayer(new SceneLayer(0, 1.f, 1.f));
}

Scene::~Scene() { Scene::instance_counter--; for (SceneLayer* L : this->scene_layers) delete L; }

//Getters
Camera* Scene::getCamera()
{
	return scene_camera;
}

std::vector<Object*> Scene::getObjectsWithinCamera(signed int layer_num)
{
	for (SceneLayer* layer : this->scene_layers)
		if (layer->getLayerNum() == layer_num)
			return layer->getLayerObjectsWithinView(this->scene_camera->getCameraViewRect());
}

std::vector<Layout*> Scene::getLayoutsWithinCamera(signed int layer_num)
{
	for (SceneLayer* layer : this->scene_layers)
		if (layer->getLayerNum() == layer_num)
			return layer->getLayerLayoutsWithinView(this->scene_camera->getCameraViewRect());
}

Object* Scene::getObjectWithId(int id)
{
	for (auto& obj : this->scene_objects)
	{
		if (obj->getId() == id)
		{
			return obj;
		}
	}

	std::cout << "ERROR: no object with id " << id << "in scene" << std::endl;
	return nullptr;
}

CollisionDetection* Scene::getCollisionDetection()
{
	return this->col_det;
}

SceneLayer* Scene::getLayer(int layer_num)
{
	for (SceneLayer* sl : this->scene_layers)
	{
		if (sl->getLayerNum() == layer_num) return sl;
	}

	std::cout << "ERROR: SceneLayer* Scene::getLayer(int layer_num), no scene_layer with num = " << layer_num << " in this scene." << std::endl;

	return nullptr;
}

//Setters
void Scene::setCamera(Camera* camera)
{
	scene_camera = camera;
}

void Scene::setCollisionDetection(CollisionDetection* col)
{
	this->col_det = col;
}

void Scene::setPhysicsManager(PhysicsManager* phys)
{
	this->phys_mag = phys;
}

//Etc

void Scene::addSceneLayer(SceneLayer* L)
{
	if (!this->addLayer(L))
		delete L;
}

void Scene::addSceneObject(Object* object)
{
	scene_objects.push_back(object);
	this->addObjectToSceneLayer(object, 0);
}

void Scene::addSceneObjects(std::vector<Object*> objects)
{
	scene_objects.insert(scene_objects.end(), objects.begin(), objects.end());
	this->addObjectsToSceneLayer(objects, 0);
}

void Scene::addObjectToSceneLayer(Object* object, int layer_num)
{
	//Finds the SceneLayer with the corresponding layer_num in vector and adds object to it.
	for (SceneLayer* layer : this->scene_layers)
	{
		if (layer->getLayerNum() == layer_num)
		{
			layer->addObject(object);
			break;
		}
	}
}

void Scene::addObjectsToSceneLayer(std::vector<Object*> objects, int layer_num)
{
	//Fins the SceneLayer with the corresponding layer_num in vector and inserts the object vector into it.
	for (SceneLayer* layer : this->scene_layers)
	{
		if (layer->getLayerNum() == layer_num)
		{
			layer->addObjects(objects);
			break;
		}
	}
}

void Scene::updateSceneFrame()
{
	if(phys_mag != nullptr)
		phys_mag->basicCollisionHandler(scene_camera->getCameraViewRect());
	for (SceneLayer* L : this->scene_layers)
		L->updateLayerObjects();
}
std::vector<sf::View> Scene::getLayerManipulatedViews()
{
	std::vector<sf::View> manip_v;
	for (SceneLayer* L : this->scene_layers) 
		manip_v.push_back(L->manipulateCameraView(*this->scene_camera->getCameraView()));
	return manip_v;
}
std::map<int, sf::View> Scene::getLayerManipulatedView()
{
	std::map<int, sf::View> num_view_pairs;
	for (SceneLayer* L : this->scene_layers)
	{
		std::pair<int, sf::View> nvp;
		nvp.first = L->getLayerNum();
		nvp.second = L->manipulateCameraView(*this->scene_camera->getCameraView());
		num_view_pairs.insert(nvp);
	}
	return num_view_pairs;
}
bool Scene::addLayer(SceneLayer* layer)
{
	for (int i = 0; i < this->scene_layers.size(); i++)
	{
		//Vector can't contain layer_num duplicates.
		if (this->scene_layers[i]->getLayerNum() == layer->getLayerNum())
			return false;
		//If an element has a lower layer_num than layer (param), layer is inserted right before that element.
		else if (this->scene_layers[i]->getLayerNum() < layer->getLayerNum())
		{
			this->scene_layers.insert(scene_layers.begin() + i, layer);
			this->layer_nums.insert(this->layer_nums.begin() + i, layer->getLayerNum());
			return true;
		}
	}
	//Runs if scene_layers is empty or if layer has the smallest layer_num, thus adding it to the back of the vector.
	this->scene_layers.push_back(layer);
	this->layer_nums.push_back(layer->getLayerNum());
	return true;
}