#include "TextureManager.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
	for (int i = 0; i < atlases.size(); i++)
	{
		delete atlases[i];
	}
	atlases.resize(0);
}

//Getters
TextureAtlas* TextureManager::getAtlas(sf::String name)
{
	int index;
	if (this->exists(name))
	{
		index = search_index;
		search_index = -1;
		return atlases[index];
	}
	return nullptr;
}

//Setters

//Others
void TextureManager::loadAtlas(sf::String name, sf::String file_name)
{
	if (!this->exists(name))
	{
		TextureAtlas* new_atlas = new TextureAtlas(name, file_name);
		if (!(new_atlas->isLoaded()))
		{
			delete new_atlas;
			return;
		}
		atlases.push_back(new_atlas);
		return;
	}
	search_index = -1;
}

bool TextureManager::exists(sf::String name)
{
	for (int i = 0; i < atlases.size(); i++)
	{
		if (name == atlases[i]->getAtlasName())
		{
			search_index = i;
			return true;
		}
	}
	search_index = -1;
	return false;
}