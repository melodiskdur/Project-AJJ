#include "../includes/ResourceLoader.h"

using namespace ResourceLoader;

TextureManager* ResourceLoader::loadResources()
{
	TextureManager* tex_mag = new TextureManager();
	tex_mag->loadAtlas("Rogue", "../Project_AJJ/assets/rogue_atlas.png");
	tex_mag->loadAtlas("Dirt Tiles", "../Project_AJJ/assets/platformertiles.png");
	tex_mag->loadAtlas("Boss", "../Project_AJJ/assets/boss.PNG");

	//Set up Rogue.
	TextureAtlas* rogue = tex_mag->getAtlas("Rogue");
	rogue->createRegionGrid(10, 10);
	rogue->assignTextureId(TEXTURE_ID::RUN, sf::Vector2u(0, 2), sf::Vector2u(9, 2));

	//Set up Dirt Tiles.
	TextureAtlas* dirt_tiles = tex_mag->getAtlas("Dirt Tiles");
	dirt_tiles->createRegionGrid(3, 8);
	dirt_tiles->assignRegionNames("Tile", 0, 2);

	//Set up Boss.
	TextureAtlas* boss = tex_mag->getAtlas("Boss");
	boss->createRegionGrid(1, 1);

	return tex_mag;
}