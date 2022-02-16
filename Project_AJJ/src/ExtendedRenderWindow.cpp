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
	if (this->window_state)
	{
		// clear the window with transparent(black) color
		this->clear(sf::Color::Transparent);

		if (active_scene != nullptr)
		{
			this->active_scene->updateSceneFrame();							 //Collision detection.
			//All layers are drawn.
			this->setView(*(active_scene->getCamera()->getCameraView()));	 //Updates the sf::View position.
			this->drawLayers();
		}

		// Debugging.
		if (this->debugger_mode)
			this->debugDraw();

		// end the current frame. Display all changes
		this->display();
	}
	else
	{
		sf::Text text;
		sf::Font font;
		font.loadFromFile("../Project_AJJ/assets/ayar_takhu.ttf");
		text.setString("PAUSED");
		text.setFont(font);
		text.setCharacterSize(34); // in pixels, not points!
		text.setFillColor(sf::Color::White);
		text.setPosition({ this->getActiveScene()->getCamera()->getCameraViewRect().left + 10,
							this->getActiveScene()->getCamera()->getCameraViewRect().top });

		this->draw(text);
		// end the current frame. Display all changes
		this->display();
	}
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

		//Camera-view must be equal to the active_scene's camera-view.
		this->scene_layer_textures[i]->setView(*(active_scene->getCamera()->getCameraView()));

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

			if (layers[i]->layer_num == 0)
			{
				//Draws object directly onto RenderWindow if main scene layer.
				//DEBUGGING
				this->draw(obj_gs);		  
				this->draw(o->vel_vec[0]);
				this->draw(o->vel_vec[1]);
				this->draw(o->vel_vec[2]);
				this->draw(o->vel_vec[3]);	
				//END DEBUGGING
				this->draw(obj_sprite);
			}
			else
			{		
				//Draws object to RenderTexture if any other layer.
				this->scene_layer_textures[i]->draw(obj_sprite);
			}
		}

		//Converts all layer textures (except main scene layer (0) ) to sprites and draws them onto RenderWindow.
		if (layers[i]->layer_num != 0)
		{
			//Converts texture to a sprite.
			sf::Sprite layer_sprite(this->scene_layer_textures[i]->getTexture());
			//Sets texture sprite-position to equal the camera view_rect (what we see on screen).

			//Scale each render texture
			layer_sprite.move(this->active_scene->getCamera()->getCameraViewRect().left, this->active_scene->getCamera()->getCameraViewRect().top);
			float zoom_factor = this->active_scene->getCamera()->getCameraZoom();
			layer_sprite.scale(sf::Vector2f({ zoom_factor, zoom_factor}));

			//Texture sprite is drawn onto ExtenderRenderWindow for rendering onto the window screen.
			this->draw(layer_sprite);

			//Renders the texture.
			this->scene_layer_textures[i]->display();
		}
	}
}

// Debugging.
void ExtendedRenderWindow::debugDraw()
{
	
	// COLLISIONDETECTION GRID LINES
	if (this->active_scene != nullptr)
	{
		std::vector<sf::VertexArray> grid = this->active_scene->getCollisionDetection()->getGrid();
		for (int i = 0; i < grid.size(); i++)
		{
			this->draw(grid[i]);
		}
	}
	
	
	/*
	// COLLISIONGRAPH EDGES.
	if (this->active_scene != nullptr)
	{
		std::vector<sf::VertexArray> edges = this->active_scene->getPhysicsManager()->col_graph->getTree();
		for (int i = 0; i < edges.size(); i++)
		{
			this->draw(edges[i]);
		}
	}
	*/
}