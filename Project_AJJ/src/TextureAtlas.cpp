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

TextureAtlas::TextureAtlas(sf::String name, sf::String atlas_file_name)
{
	//constructor that loads an atlas directly
	this->loadAtlas(name, atlas_file_name);
}

TextureAtlas::~TextureAtlas()
{

}

//Getters
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

sf::Sprite TextureAtlas::getSprite(TEXTURE_ID texture_id, int frame_index)
{
	int sprite_index = 0;
	
	if (texture_id == TEXTURE_ID::NONE)
	{
		this->sprite.setTextureRect(sf::IntRect(0,0,0,0));
		return this->sprite;
	}

	for (int i = 0; i < texture_regions.size(); i++)
	{
		if (texture_regions[i]->texture_id == texture_id)
		{
			if (i + frame_index < texture_regions.size() &&
				texture_regions[i + frame_index]->texture_id == texture_id)
				sprite_index = i + frame_index;
			else
				sprite_index = i;
			break;
		}
	}
	this->sprite.setTextureRect(texture_regions[sprite_index]->region_rect);
	return this->sprite;
}

sf::Sprite TextureAtlas::getSprite(sf::String region_name, int frame_index)
{
	int sprite_index = 0;
	for (int i = 0; i < texture_regions.size(); i++)
	{
		if (texture_regions[i]->region_name == region_name)
		{
			if (i + frame_index < texture_regions.size() &&
				texture_regions[i + frame_index]->region_name == region_name)
				sprite_index = i + frame_index;
			else
				sprite_index = i;
			break;
		}
	}
	this->sprite.setTextureRect(texture_regions[sprite_index]->region_rect);
	return this->sprite;
}

sf::String TextureAtlas::getAtlasName()
{
	return atlas_name;
}

//Setters

//Others
void TextureAtlas::loadAtlas(sf::String name, sf::String atlas_file_name)
{
	//Create a new atlas and load it from file
	sf::Texture* new_atlas = new sf::Texture;
	if (!new_atlas->loadFromFile(atlas_file_name))
	{
		std::cout << "ERROR: TextureHandler constructor, could'nt load atlas" << std::endl;
		return;
	}

	//Check if there already is an atlas. If so, delete it
	if (!(this->atlas == nullptr))
		this->atlas->~Texture();

	//set the new_atlas to this atlas
	this->atlas = new_atlas;

	//set sprite texture to the newly created atlas
	this->sprite.setTexture(*new_atlas);
	this->atlas_name = name;
	this->file_name = atlas_file_name;
	this->atlas_loaded = true;
}

bool TextureAtlas::isLoaded()
{
	return atlas_loaded;
}

void TextureAtlas::clearTextureRegions()
{
	for (int i = 0; i < texture_regions.size(); i++)
		delete texture_regions[i];
	texture_regions.resize(0);
}

void TextureAtlas::createRegionGrid(int rows, int columns)
{
	if (atlas == nullptr)
		return;
	this->rows = rows;
	this->columns = columns;
	this->clearTextureRegions();
	int posx, posy;
	sf::Vector2u tex_size = atlas->getSize();
	int width = (int) (tex_size.x / columns);
	int height = (int) (tex_size.y / rows);
	for (int i = 0; i < rows; i++)
	{
		posy = i * height;
		for (int j = 0; j < columns; j++)
		{
			posx = j * width;
			Region* temp = new Region;
			temp->region_rect = sf::IntRect(posx, posy, width, height);
			temp->texture_id = TEXTURE_ID::NONE;
			temp->frame_index = 0;
			texture_regions.push_back(temp);
		}
	}
}

void TextureAtlas::assignTextureId(TEXTURE_ID id, int first_row, int last_row)
{
	if (first_row > last_row || last_row > this->rows)
		return;
	int frame_index = 0;
	for (int i = first_row; i < last_row + 1; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			texture_regions[j +  i * this->columns]->texture_id = id;
			texture_regions[j + i * this->columns]->frame_index = frame_index;
			frame_index++;
		}
	}
}

void TextureAtlas::assignTextureId(TEXTURE_ID id, int row)
{
	this->assignTextureId(id, row, row);
}

void TextureAtlas::assignTextureId(TEXTURE_ID id, sf::Vector2u first_cell, sf::Vector2u last_cell)
{
	if (first_cell.x > this->columns || first_cell.y > this->rows ||
		last_cell.x > this->columns || last_cell.y > this->rows ||
		first_cell.y > last_cell.y || (first_cell.y == last_cell.y && first_cell.x > last_cell.x))
		return;

	int frame_index = 0;
	int first_index = first_cell.x + this->columns * first_cell.y;
	int last_index = last_cell.x + this->columns * last_cell.y;
	for (int i = first_index; i < last_index; i++)
	{
		texture_regions[i]->texture_id = id;
		texture_regions[i]->frame_index = frame_index;
		frame_index++;
	}
}

void TextureAtlas::assignRegionName(sf::String region_name, sf::Vector2u tex_cell, int frame_index)
{
	if (tex_cell.x > this->columns || tex_cell.y > this->rows)
		return;
	texture_regions[tex_cell.x + this->columns * tex_cell.y]->region_name = region_name;
	texture_regions[tex_cell.x + this->columns * tex_cell.y]->frame_index = frame_index;
}

void TextureAtlas::assignRegionNames(sf::String region_name, int first_row, int last_row)
{
	if (first_row > last_row || last_row > this->rows)
		return;
	int frame_index = 0;
	for (int i = first_row; i < last_row + 1; i++)
	{
		for (int j = 0; j < this->columns; j++)
		{
			this->assignRegionName(region_name, sf::Vector2u(j, i), frame_index);
			frame_index++;
		}
	}
}