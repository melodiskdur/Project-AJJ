#include "TextureAtlas.h"


/*Texture Regions
* Used together with the textureid enums to retrieve specific regions
* of the texture atlas. We want to hardcode the areas since the sprites 
* could be very different from eachother in the atlas.
*/
const sf::IntRect TEXTURE_REGIONS[10] =
{
  sf::IntRect { 0, 0, 0, 0 }, // NONE

  sf::IntRect { 1340, 20, 190, 300 },
  sf::IntRect { 1660, 20, 190, 300 },
  sf::IntRect { 1980, 20, 190, 300 },
  sf::IntRect { 2300, 20, 190, 300 },
  sf::IntRect { 2620, 20, 190, 300 },
  sf::IntRect { 2940, 20, 190, 300 },
  sf::IntRect { 3260, 20, 190, 300 },
  sf::IntRect { 3580, 20, 190, 300 },
  sf::IntRect { 2300, 20, 190, 300 }

};

TextureAtlas::TextureAtlas()
{

}

TextureAtlas::TextureAtlas(sf::String atlas_file_name)
{
	//constructor that loads an atlas directly
	this->loadAtlas(atlas_file_name);
}

TextureAtlas::~TextureAtlas()
{

}

void TextureAtlas::loadAtlas(sf::String atlas_file_name)
{
	//Create a new atlas and load it from file
	sf::Texture* new_atlas = new sf::Texture;
	if (!new_atlas->loadFromFile(atlas_file_name))
		std::cout << "ERROR: TextureHandler constructor, could'nt load atlas" << std::endl;

	//Check if there already is an atlas. If so, delete it
	if (!(this->atlas == nullptr))
		this->atlas->~Texture();

	//set the new_atlas to this atlas
	this->atlas = new_atlas;

	//set sprite texture to the newly created atlas
	this->sprite.setTexture(*new_atlas);
}

int TextureAtlas::getWidth(TEXTURE_ID texture_id)
{
	return TEXTURE_REGIONS[static_cast<int>(texture_id)].width;
}

int TextureAtlas::getHeight(TEXTURE_ID texture_id)
{
	return TEXTURE_REGIONS[static_cast<int>(texture_id)].height;
}

sf::Sprite TextureAtlas::getSprite(TEXTURE_ID texture_id)
{
	//return a specific area of the sprite sheet/texture atlas
	this->sprite.setTextureRect(TEXTURE_REGIONS[static_cast<int>(texture_id)]);
	return this->sprite;
}

