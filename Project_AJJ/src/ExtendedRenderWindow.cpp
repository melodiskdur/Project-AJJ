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
	this->clearSceneLayerTextures();
	int num_layers = active_scene->getSceneLayers().size();
	for (int i = 0; i < num_layers; i++)
	{
		sf::RenderTexture* t_new = new sf::RenderTexture();
		if (t_new->create(this->getSize().x, this->getSize().y))
			this->scene_layer_textures.push_back(t_new);
	}
}

void ExtendedRenderWindow::setTextureManager(TextureManager* tex_mag)
{
	this->texture_manager = tex_mag;
}

void ExtendedRenderWindow::drawActiveScene()
{
	if (active_scene != nullptr)
	{
		//All layers are drawn.
		this->drawLayers();
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

//Private functions.
void ExtendedRenderWindow::clearSceneLayerTextures()
{
	for (sf::RenderTexture* t : this->scene_layer_textures)
		delete t;
	this->scene_layer_textures.resize(0);
}

void ExtendedRenderWindow::drawLayers()
{
	this->active_scene->updateSceneLayers();
	std::vector<SceneLayer*>& layers = this->active_scene->getSceneLayers(); //For readability.
	for (int i = 0; i < this->scene_layer_textures.size(); i++)
	{
		//Clears each RenderTexture in preparation for rendering.
		this->scene_layer_textures[i]->clear(sf::Color::Transparent);

		//Calculates the Drawable and positioning of each object in each layer.
		for (Object* o : layers[i]->layer_objects)
		{
			TextureAtlas* obj_atlas = this->texture_manager->getAtlas(o->getTextureName());	//For readability.
			if (obj_atlas == nullptr)
				continue;

			//Prepare sprite and geo_shape of current object.
			sf::Sprite obj_sprite = obj_atlas->getSprite(o->getFrame().texture_id, o->getFrame().region_name, o->getFrame().frame_index);
			obj_sprite.setPosition(o->getWorldPosition());
			sf::VertexArray obj_gs = o->getGeoShape();
			sf::FloatRect sprite_r = obj_sprite.getLocalBounds();
			//Scaling to fit within object boundaries.
			obj_sprite.scale((obj_gs[1].position.x - obj_gs[0].position.x) / (sprite_r.width), (obj_gs[2].position.y - obj_gs[1].position.y) / (sprite_r.height));
			//Draws object to RenderTexture.
			this->scene_layer_textures[i]->draw(obj_sprite);
		}

		//Camera-view must be equal to the active_scene's camera-view.
		this->scene_layer_textures[i]->setView(*(active_scene->getCamera()->getCameraView()));
		//Renders the texture.
		this->scene_layer_textures[i]->display();
		//Converts texture to a sprite.
		sf::Sprite layer_sprite(this->scene_layer_textures[i]->getTexture());
		//Sets texture sprite-position to equal the camera view_rect (what we see on screen).
		layer_sprite.move(this->active_scene->getCamera()->getCameraViewRect().left, this->active_scene->getCamera()->getCameraViewRect().top);
		//Texture sprite is drawn onto ExtenderRenderWindow for rendering onto the window screen.
		this->draw(layer_sprite);
	}
}