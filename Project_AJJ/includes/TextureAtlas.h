#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureIds.h"

/* Region
*  Offers a way to cut certain regions of a TextureAtlas to use 
*  as sprites. A Region can be labeled with a texture_id which
*  is useful if the TextureAtlas contains animation sequences. For example,
*  label the TextureAtlas rows, that correspond to a running animation, with
*  TEXTURE_ID::RUN. You can then call getSprite(RUN) to get the correct region
*  of the atlas as a sprite.
* 
*  Region also has a string region_name that can be used to retreive certain regions
*  as sprites. This is useful for non-animated or custom animated sprites.
*/
typedef struct _Region
{
	sf::IntRect region_rect;
	TEXTURE_ID texture_id = TEXTURE_ID::NONE;
	sf::String region_name = "";
	int frame_index = 0;

} Region;

/*TextureAtlas
* For loading an atlas and retrieving sprites
*/
class TextureAtlas
{
public:
	//Constructors
	TextureAtlas();//default
	TextureAtlas(sf::String name, sf::String atlas_file_name);//uses the loadAtlas method

	//Destructor
	virtual ~TextureAtlas();//default

	//Getters
	int getWidth(TEXTURE_ID texture_id);
	int getHeight(TEXTURE_ID texture_id);
	sf::Sprite getSprite(TEXTURE_ID texture_id, sf::String region_name, int frame_index);
	sf::Sprite getSprite(TEXTURE_ID texture_id);
	sf::Sprite getSprite(TEXTURE_ID texture_id, int frame_index);
	sf::Sprite getSprite(sf::String region_name, int frame_index);
	sf::String getAtlasName();
	int getNumRows() { return this->rows; }
	int getNumColumns() { return this->columns; }

	//Setters

	//Others
	//loads an texture atlas from file
	void loadAtlas(sf::String name, sf::String atlas_file_name);
	//Checks if a texture was loaded correctly (e.g a corresponding file exists).
	bool isLoaded();
	//Frees memory of the texture_regions vector.
	void clearTextureRegions();
	//Creates a 'rows'x'columns' grid of regions from a texture.
	void createRegionGrid(int rows, int columns);

	//createRegion(sf::IntRect region_rect);

	//Useful for animations.
	void assignTextureId(TEXTURE_ID id, int first_row, int last_row);
	void assignTextureId(TEXTURE_ID id, int row);
	void assignTextureId(TEXTURE_ID, sf::Vector2u first_cell, sf::Vector2u last_cell);
	//Useful for static sprites, but can also be used for animations.
	void assignRegionName(sf::String region_name, sf::Vector2u tex_cell, int frame_index);
	void assignRegionNames(sf::String region_name, int first_row, int last_row);

	static unsigned int instanceCount() { return instance_counter; };
private:
	sf::String atlas_name = "";
	sf::String file_name = "";
	sf::Texture* atlas = nullptr;//Texture atlas
	int rows = 0;
	int columns = 0;
	sf::Sprite sprite; //a drawable sprite. Its texture will be the atlas
	//sf::IntRect size, dest;
	std::vector<Region*> texture_regions;

	bool atlas_loaded = false;

	static unsigned int instance_counter;
protected:
};