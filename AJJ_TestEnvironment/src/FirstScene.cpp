#include "../includes/FirstScene.h"
#include "../includes/TestLayout.h"

using namespace FirstScene;

//10x35
static std::vector<int> world_map =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	-1, -1, -1, -1, -1,	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	-1, -1, -1, -1, -1,	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 1,  1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	-1, -1, -1, -1, -1,	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	-1, -1, -1, -1, -1,	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	-1, -1, -1, -1, -1,	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	-1, -1, -1, -1, -1,	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,	-1, -1, -1, -1, -1,	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 9,  9,  5,  1,  1,  1,  2, -1,  0,  1,  1,  1,  1,  1,  1,	 1,  1,  1,  2, -1,	-1,  0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
	 9,  9,  9,  9,  9,  9, 10, -1,  8,  9,  9,  9,  9,  9,  9,	 9,  9,  9, 10, -1,	-1,  8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,
	 9,  9,  9,  9,  9,  9, 10, -1,  8,  9,  9,  9,  9,  9,  9,	 9,  9,  9, 10, -1,	-1,  8,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9,  9
};

//10x35
static std::vector<int> bg_layer1 =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  1,  1,  1,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  0,  2, -1, -1,  8,  9,  9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  8, 10, -1, -1,  8,  9,  9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  8, 10, -1, -1,  8,  9,  9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  8, 10, -1, -1,  8,  9,  9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  8, 10, -1, -1,  8,  9,  9,  9, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

//10x25
static std::vector<int> bg_layer_1_5 =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

//13x25
static std::vector<int> bg_layer2 =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, 14, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 13, 14, -1, -1, -1, 14, -1, -1, -1, 13, 14, 15, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 14, 13, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

//18x34
static std::vector<int> bg_layer3 =
{
	11, 11, 11, 11, 11, 11,
	11, 11, 11, 11, 11, 11,
	11, 11, 11, 11, 11, 11,
	11, 11, 11, 11, 11, 11,
	11, 11, 11, 11, 11, 11,
	11, 11, 11, 11, 11, 11
};

//10x35
static std::vector<int> fg_layer1 =
{
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,  0,  2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  0,  2, -1, -1, -1, -1, -1, -1, -1, -1,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  8, 10, -1, -1, -1, -1,  0,  2, -1, -1,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  8, 10, -1, -1, -1, -1,  8, 10, -1, -1,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  8, 10, -1, -1, -1, -1,  8, 10, -1, -1,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1,  8, 10, -1, -1, -1, -1,  8, 10, -1, -1,  8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

Scene* FirstScene::createScene()
{
	Scene* first_scene = new Scene();
	FirstScene::createWorldLayer(first_scene, "Dirt Tiles", world_map, 10, 35, sf::Vector2f(-50.0f, -350.0f), sf::Vector2f(80.0f, 80.0f));
	FirstScene::createBackgroundLayer(first_scene, "Dirt Tiles", bg_layer1, 10, 35, 1, 1.3f, 0.9f, sf::Vector2f(-50.0f, -350.0f), sf::Vector2f(80.0f, 80.0f), "Tile");
	//FirstScene::createBackgroundLayer(first_scene, "Dirt Tiles", bg_layer2, 13, 25, 9, 45.0f, 0.8f, sf::Vector2f(50.0f, -350.0f), sf::Vector2f(80.0f, 80.0f), "Tile");
	FirstScene::createMountain(first_scene);
	FirstScene::createForestbg(first_scene);
	FirstScene::createCloudLayer(first_scene);
	FirstScene::createCloudLayer2(first_scene);
	FirstScene::createStaticCloudBG(first_scene);
	FirstScene::createCloudLayer3(first_scene);
	//FirstScene::createStaticBackgroundLayer(first_scene, "Dirt Tiles", bg_layer3, 6, 6, 9, 1.f, 10.f, "Tile");
	FirstScene::createBackgroundLayer(first_scene, "Dirt Tiles", fg_layer1, 10, 35, -1, 0.5f, 1.7f, sf::Vector2f(200.0f, -400.0f), sf::Vector2f(80.0f, 80.0f), "Tile");
	FirstScene::addTrees(first_scene);
	
	SceneLayer* layout_test1 = new SceneLayer(-2, 1.f, 0);
	layout_test1->addLayout(TestLayout::createTestLayout(false));
	first_scene->addSceneLayer(layout_test1);

	SceneLayer* layout_test2 = new StaticFixatedLayer(-3, 1.f, sf::View({ 0.f, 0.f }, { 800.f, 600.f }));
	layout_test2->addLayout(TestLayout::createTestLayout(true));

	first_scene->addSceneLayer(layout_test2);


	return first_scene;
}


void FirstScene::createWorldLayer(Scene* scene, sf::String texture_name, std::vector<int> map, int rows,
	                              int cols, sf::Vector2f start_pos, sf::Vector2f object_size)
{
	float current_x;
	float current_y;
	int counter = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i * cols + j] == -1)
				continue;
			
			current_x = start_pos.x + j * object_size.x;
			current_y = start_pos.y + i * object_size.y;
			Object* tile = new Object(sf::Vector2f(current_x, current_y), object_size);
			tile->setTextureName(texture_name);
			tile->setId(counter);
			Frame tile_frame;
			tile_frame.duration = 200;
			tile_frame.frame_index = map[i * cols + j];
			tile_frame.texture_id = TEXTURE_ID::NONE;
			tile_frame.region_name = "Tile";
			tile->setCurrentFrame(tile_frame);
			scene->addSceneObject(tile);
			counter++;
		}
	}
}

void FirstScene::createBackgroundLayer(Scene* scene, sf::String texture_name, std::vector<int> map, int rows, int cols,
								       int layer_num, float layer_depth, float layer_scale, sf::Vector2f start_pos, sf::Vector2f object_size, sf::String region_name)
{
	float current_x;
	float current_y;

	SceneLayer* new_layer = new StaticInteractiveLayer(layer_num, layer_scale, layer_depth);
	scene->addSceneLayer(new_layer);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i * cols + j] == -1)
				continue;

			current_x = start_pos.x + j * object_size.x;
			current_y = start_pos.y + i * object_size.y;
			Object* tile = new Object(sf::Vector2f(current_x, current_y), object_size);
			tile->setTextureName(texture_name);
			Frame tile_frame;
			tile_frame.duration = 200;
			tile_frame.frame_index = map[i * cols + j];
			tile_frame.texture_id = TEXTURE_ID::NONE;
			tile_frame.region_name = region_name;
			tile->setCurrentFrame(tile_frame);
			scene->addObjectToSceneLayer(tile, layer_num);
		}
	}
}

void FirstScene::createStaticBackgroundLayer(Scene* scene, sf::String texture_name, std::vector<int> map, int rows, int cols,
	int layer_num, float layer_depth, float layer_scale, sf::String region_name)
{
	float current_x;
	float current_y;
	sf::Vector2f start_pos = {-500.f, -400.f};
	sf::Vector2f object_size = {200.f, 200.f};

	SceneLayer* layer = new StaticFixatedLayer(layer_num, 10.f, sf::View({ 0.f, 0.f }, { 800.f, 600.f }));
	scene->addSceneLayer(layer);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (map[i * cols + j] == -1)
				continue;

			current_x = start_pos.x + j * object_size.x;
			current_y = start_pos.y + i * object_size.y;
			Object* tile = new Object(sf::Vector2f(current_x, current_y), object_size);
			tile->setTextureName(texture_name);
			Frame tile_frame;
			tile_frame.duration = 200;
			tile_frame.frame_index = map[i * cols + j];
			tile_frame.texture_id = TEXTURE_ID::NONE;
			tile_frame.region_name = region_name;
			tile->setCurrentFrame(tile_frame);
			layer->addObject(tile);
		}
	}
}

void FirstScene::createCloudLayer(Scene* scene)
{
	DynamicFixatedLayer* cloud_layer = new DynamicFixatedLayer(8, 30.f, 2.7f);
	cloud_layer->setAutoScrolling({50.3f, 0.f});
	std::vector<sf::Vector2f> positions = { {-430.f, -150.f}, { -200.f, 80.f}, { 450.f, -400.f}, { 130.f, -350.f} };
	scene->addSceneLayer(cloud_layer);
	for (int i = 0; i < 4; i++)
	{
		Object* c = new Object(positions[i], sf::Vector2f(262.f, 128.f));
		c->setTextureName("Bigcloud");
		Frame frame;
		frame.duration = 200;
		frame.frame_index = 0;
		frame.texture_id = TEXTURE_ID::NONE;
		frame.region_name = "Bigcloud";
		c->setCurrentFrame(frame);
		cloud_layer->addObject(c);
	}
}

void FirstScene::createCloudLayer2(Scene* scene)
{
	DynamicFixatedLayer* cloud_layer = new DynamicFixatedLayer(7, 30.f, 2.7f);
	cloud_layer->setAutoScrolling({ 100.6f, 0.f });
	std::vector<sf::Vector2f> positions = { {-30.f, -130.f}, { -230.f, -400.f}, { 450.f, -35.f}, { 130.f, -250.f} };
	scene->addSceneLayer(cloud_layer);
	for (int i = 0; i < 4; i++)
	{
		Object* c = new Object(positions[i], sf::Vector2f(262.f, 128.f));
		c->setTextureName("Smallcloud");
		Frame frame;
		frame.duration = 200;
		frame.frame_index = 0;
		frame.texture_id = TEXTURE_ID::NONE;
		frame.region_name = "Smallcloud";
		c->setCurrentFrame(frame);
		cloud_layer->addObject(c);
	}
}

void FirstScene::createMountain(Scene* scene)
{
	StaticInteractiveLayer* L = new StaticInteractiveLayer(6, 1.f, 20.7f);
	scene->addSceneLayer(L);
	Object* c = new Object(sf::Vector2f(-300.f, -350.f), sf::Vector2f(4.f * 225.f, 4.f * 138.f));
	c->setTextureName("Mountain");
	Frame frame;
	frame.duration = 200;
	frame.frame_index = 0;
	frame.texture_id = TEXTURE_ID::NONE;
	frame.region_name = "Mountain";
	c->setCurrentFrame(frame);
	L->addObject(c);
}

void FirstScene::createCloudLayer3(Scene* scene)
{
	StaticInteractiveLayer* L = new StaticInteractiveLayer(9, 1.f, 50.f);
	scene->addSceneLayer(L);
	Object* c = new Object(sf::Vector2f(-500.f, -500.f), sf::Vector2f(4.f * 360.f, 4.f * 136.f));
	c->setTextureName("Highclouds");
	Frame frame;
	frame.duration = 200;
	frame.frame_index = 0;
	frame.texture_id = TEXTURE_ID::NONE;
	frame.region_name = "Highclouds";
	c->setCurrentFrame(frame);
	L->addObject(c);
}

void FirstScene::createForestbg(Scene* scene)
{
	StaticInteractiveLayer* L = new StaticInteractiveLayer(5, 1.f, 10.7f);
	scene->addSceneLayer(L);
	Object* c = new Object(sf::Vector2f(-600.f, -100.f), sf::Vector2f(4.f * 500.f, 4.f * 100.f));
	c->setTextureName("Foresttowerbg");
	Frame frame;
	frame.duration = 200;
	frame.frame_index = 0;
	frame.texture_id = TEXTURE_ID::NONE;
	frame.region_name = "Foresttowerbg";
	c->setCurrentFrame(frame);
	L->addObject(c);
}

void FirstScene::createStaticCloudBG(Scene* scene)
{
	SceneLayer* L = new StaticFixatedLayer(10, 1.f, sf::View({ 0.f, 0.f }, { 800.f, 600.f }));
	scene->addSceneLayer(L);
	Object* c = new Object(sf::Vector2f(-400.f, -400.f), sf::Vector2f(800.f, 800.f));
	c->setTextureName("Cloudsbg");
	Frame frame;
	frame.duration = 200;
	frame.frame_index = 0;
	frame.texture_id = TEXTURE_ID::NONE;
	frame.region_name = "Cloudsbg";
	c->setCurrentFrame(frame);
	L->addObject(c);
}

void FirstScene::addTrees(Scene* scene)
{
	StaticInteractiveLayer* L = new StaticInteractiveLayer(3, 1.f, 3.7f);
	scene->addSceneLayer(L);
	for (int i = 0; i < 50; i++)
	{
		Object* c = new Object(sf::Vector2f(-400.f + (float)i*200.f, 0.f), sf::Vector2f(4.f * 72.f, 4.f * 136.f));
		c->setTextureName("Tree");
		Frame frame;
		frame.duration = 200;
		frame.frame_index = 0;
		frame.texture_id = TEXTURE_ID::NONE;
		frame.region_name = "Tree";
		c->setCurrentFrame(frame);
		L->addObject(c);
	}

	L = new StaticInteractiveLayer(4, 1.f, 5.7f);
	scene->addSceneLayer(L);
	for (int i = 0; i < 50; i++)
	{
		Object* c = new Object(sf::Vector2f(-400.f + (float)i * 200.f, -50.f), sf::Vector2f(4.f * 72.f, 4.f * 136.f));
		c->setTextureName("Tree");
		Frame frame;
		frame.duration = 200;
		frame.frame_index = 0;
		frame.texture_id = TEXTURE_ID::NONE;
		frame.region_name = "Tree";
		c->setCurrentFrame(frame);
		L->addObject(c);
	}
}
