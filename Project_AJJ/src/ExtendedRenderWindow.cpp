#include "ExtendedRenderWindow.h"

ExtendedRenderWindow::ExtendedRenderWindow(sf::Vector2u resolution, std::string window_title)
	: sf::RenderWindow(sf::VideoMode(resolution.x, resolution.y), window_title)
{

}

ExtendedRenderWindow::~ExtendedRenderWindow()
{
	std::cout << "ExtendedRenderWindow deleted" << std::endl;
}

//Getters
Scene* ExtendedRenderWindow::getActiveScene()
{
	return active_scene;
}

TextureManager* ExtendedRenderWindow::getTextureManager()
{
	return this->texture_manager;
}

//Setters
void ExtendedRenderWindow::setActiveScene(Scene* scene)
{
	active_scene = scene;
}

void ExtendedRenderWindow::setTextureManager(TextureManager* tex_mag)
{
	this->texture_manager = tex_mag;
}

void ExtendedRenderWindow::drawActiveScene()
{
	if (active_scene != nullptr)
	{
		for (Object* obj : active_scene->getSceneObjects())
		{
			this->draw(obj->getGeoShape());							 //WIP

			//------------Animation test-----------
			//get the objects sprite
			TextureAtlas* obj_atlas = this->texture_manager->getAtlas(obj->getTextureName());
			if (obj_atlas == nullptr)
				continue;

			sf::Sprite sprite = obj_atlas->getSprite(obj->getFrame().texture_id, obj->getFrame().region_name, obj->getFrame().frame_index);

			//set the position of the sprite to the same as the objects
			sprite.setPosition(obj->getWorldPosition());

			//correct scaling of the sprite so that it fits the object
			sf::VertexArray obj_gs = obj->getGeoShape();
			sf::FloatRect sprite_r = sprite.getLocalBounds();
			sprite.scale((obj_gs[1].position.x - obj_gs[0].position.x) / (sprite_r.width), (obj_gs[2].position.y - obj_gs[1].position.y) / (sprite_r.height));
			this->draw(sprite);
			//-------------------------------------

			//this->active_scene->updateSceneFrame();
		}
	}
	this->setView(*(active_scene->getCamera()->getCameraView()));	 //Updates the sf::View position.
}