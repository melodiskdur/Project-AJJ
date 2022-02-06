#include "../includes/ResourceLoader.h"

using namespace ResourceLoader;

TextureManager* ResourceLoader::loadResources()
{
	TextureManager* tex_mag = new TextureManager();
	tex_mag->loadAtlas("Rogue", "../Project_AJJ/assets/rogue_atlas.png");
	tex_mag->loadAtlas("Dirt Tiles", "../Project_AJJ/assets/platformertiles.png");
	tex_mag->loadAtlas("Boss", "../Project_AJJ/assets/boss.PNG");
	tex_mag->loadAtlas("MenuButtons", "../Project_AJJ/assets/menu_buttons.PNG");

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
	boss->assignRegionName("Boss", sf::Vector2u(0, 0), 0);

	//Set up Menu Buttons.
	TextureAtlas* menu_buttons = tex_mag->getAtlas("MenuButtons");
	menu_buttons->createRegionGrid(3, 3);
	menu_buttons->assignRegionName("MenuButtons", sf::Vector2u(2,0), 0);
	menu_buttons->assignTextureId(TEXTURE_ID::IDLE, sf::Vector2u(0, 0), sf::Vector2u(0, 0));
	menu_buttons->assignTextureId(TEXTURE_ID::HOVER, sf::Vector2u(1, 0), sf::Vector2u(1, 0));

	return tex_mag;
}