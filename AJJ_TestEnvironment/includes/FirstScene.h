#pragma once
#include "Scene.h"
#include "Object.h"

namespace FirstScene
{
	Scene* createScene();
	void createWorldLayer(Scene* scene, sf::String texture_name, std::vector<int> map, int rows, 
		                  int cols, sf::Vector2f start_pos, sf::Vector2f object_size);
	void createBackgroundLayer(Scene* scene, sf::String texture_name, std::vector<int> map, int rows, int cols, 
		                      float layer_depth, sf::Vector2f start_pos, sf::Vector2f object_size);
}

