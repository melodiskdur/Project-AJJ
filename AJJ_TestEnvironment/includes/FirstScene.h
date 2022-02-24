#pragma once
#include "Project_AJJ.h"
#include "Scene.h"
#include "SceneLayer.h"
#include "Object.h"

namespace FirstScene
{
	Scene* createScene();
	void createWorldLayer(Scene* scene, sf::String texture_name, std::vector<int> map, int rows, 
		                  int cols, sf::Vector2f start_pos, sf::Vector2f object_size);
	void createBackgroundLayer(Scene* scene, sf::String texture_name, std::vector<int> map, int rows, int cols, 
							   int layer_num, float layer_depth, float layer_scale, sf::Vector2f start_pos, sf::Vector2f object_size, sf::String region_name);
	void createStaticBackgroundLayer(Scene* scene, sf::String texture_name, std::vector<int> map, int rows, int cols,
		int layer_num, float layer_depth, float layer_scale, sf::String region_name);
	void createCloudLayer(Scene* scene);
	void createCloudLayer2(Scene* scene);
	void createCloudLayer3(Scene* scene);
	void createMountain(Scene* scene);
	void createForestbg(Scene* scene);
	void createStaticCloudBG(Scene* scene);
	void addTrees(Scene* scene);
}

