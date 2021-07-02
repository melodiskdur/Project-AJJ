#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TextureIds.h"

/*TextureAtlas
* For loading an atlas and retrieving sprites
*/

class TextureAtlas
{
public:
	//Constructors
	TextureAtlas();//default
	TextureAtlas(sf::String atlas_file_name);//uses the loadAtlas method

	//Destructor
	virtual ~TextureAtlas();//default

	//loads an texture atlas from file
	void loadAtlas(sf::String atlas_file_name);

	//Getters
	int getWidth(TEXTURE_ID texture_id);
	int getHeight(TEXTURE_ID texture_id);
	sf::Sprite getSprite(TEXTURE_ID texture_id);

	//Setters

private:

	sf::Texture* atlas = nullptr;//Texture atlas
	sf::Sprite sprite; //a drawable sprite. Its texture will be the atlas
	//sf::IntRect size, dest;

protected:
};