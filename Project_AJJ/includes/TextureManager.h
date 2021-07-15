#include <SFML/Graphics.hpp>
#include "TextureAtlas.h"

#pragma once
/* TextureManager
 * Contains and hands out pointers to all the textures
 * that are used in the scene. 
*/
class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	//Getters
	TextureAtlas* getAtlas(sf::String name);

	//Setters

	//Others
	void loadAtlas(sf::String name, sf::String file_name);

private:
	std::vector<TextureAtlas*> atlases;
	bool exists(sf::String name);
	int search_index = -1; //Index of the latest queried Atlas.
};

