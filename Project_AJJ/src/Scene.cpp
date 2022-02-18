#include "Scene.h"

Scene::Scene()
{
	this->createSceneLayer(0, 1.0f, 1.0f, LAYER_TYPE::DYNAMIC_FLEXIBLE);
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

std::vector<Object*> Scene::getObjectsWithinCamera(signed int layer_num)
{
	// Find the corresponding layer.
	std::vector<Object*> temp_vector;
	for (SceneLayer* layer : this->scene_layers)
	{
		if (layer->layer_num == layer_num)
		{
			// Collect the objects within the view_rect, scaled with respect
			// to the layer.
			sf::View view = *(scene_camera->getCameraView());
			view.zoom(layer->depth);
			sf::FloatRect view_rect = sf::FloatRect(view.getCenter() - 0.5f * view.getSize(), view.getSize());
			for (Object* o : layer->layer_objects)
			{
				if (view_rect.intersects({ o->getWorldPosition(), o->getSize() }))
					temp_vector.push_back(o);
			}
			break;
		}
	}
	return temp_vector;
}

std::vector<SceneLayer*> Scene::getSceneLayersByType(LAYER_TYPE type)
{
	std::vector<SceneLayer*> layers;
	for (SceneLayer* layer : this->scene_layers)
		if (layer->layer_type == type) layers.push_back(layer);
	return layers;
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
void Scene::createSceneLayer(signed int layer_num, float depth, float scale, LAYER_TYPE type)
{
	SceneLayer* new_layer = new SceneLayer();
	new_layer->layer_num = layer_num;
	new_layer->depth = depth;
	new_layer->scale = scale;
	new_layer->layer_type = type;
	if (!this->addLayer(new_layer))
		delete new_layer;
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
		if (layer->layer_num == layer_num)
		{
			// Scale and reposition the Object with regards to the Layer depth and scale.
			if (layer_num != 0)
			{
				object->setSize(object->getSize() * layer->depth * layer->scale);
				object->setWorldPosition(object->getWorldPosition() * layer->depth * layer->scale);
			}
			layer->layer_objects.push_back(object);
			break;
		}
	}
}

void Scene::addObjectsToSceneLayer(std::vector<Object*> objects, int layer_num)
{
	//Fins the SceneLayer with the corresponding layer_num in vector and inserts the object vector into it.
	for (SceneLayer* layer : this->scene_layers)
	{
		if (layer->layer_num == layer_num)
		{
			// Scale and reposition the Object with regards to the Layer depth and scale.
			if (layer_num != 0)
			{
				for (Object* o : objects)
				{
					o->setSize(o->getSize() * layer->depth * layer->scale);
					o->setWorldPosition(o->getWorldPosition() * layer->depth * layer->scale);
				}
			}
			layer->layer_objects.insert(scene_objects.end(), objects.begin(), objects.end());
			break;
		}
	}
}

void Scene::updateSceneFrame()
{
	if(phys_mag != nullptr)
		phys_mag->basicCollisionHandler(scene_camera->getCameraViewRect());
}

std::vector<sf::View> Scene::transformCameraViewToLayers()
{
	std::vector<sf::View> transformed_camera_views;
	if (this->scene_camera == nullptr) return transformed_camera_views;
	for (SceneLayer* layer: this->scene_layers)
	{
		// If the layer is of type STATIC, we retreive a view to with position back at (0, 0).
		// This is useful in the sense that we can draw everything related to that layer simply
		// at and around this coordinate and thus won't have to move the layer objects around
		// to accommodate for the moving of the Camera.
		if (layer->layer_type == LAYER_TYPE::STATIC_FIXATED || layer->layer_type == LAYER_TYPE::DYNAMIC_FIXATED)
			transformed_camera_views.push_back(
				this->scene_camera->getManipulatedCameraView(layer->depth, layer->scale, layer->rotation, sf::Vector2f(0.0f, 0.0f)));
		// Else we retreview a view with the same center as the Camera non manipulated view, but with manipulated parameters.
		else
			transformed_camera_views.push_back(this->scene_camera->getManipulatedCameraView(layer->depth, layer->scale, layer->rotation));
	}
	return transformed_camera_views;
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