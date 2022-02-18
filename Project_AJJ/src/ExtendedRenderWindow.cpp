#include "ExtendedRenderWindow.h"

ExtendedRenderWindow::ExtendedRenderWindow(sf::Vector2u resolution, std::string window_title, sf::ContextSettings settings)
	: sf::RenderWindow(sf::VideoMode(resolution.x, resolution.y), window_title, sf::Style::Default, settings)
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

Scene* ExtendedRenderWindow::getSceneFromDenotation(SCENE_DENOTATION scene_denotation)
{
	for (auto& scn : this->scenes)
	{
		if (scn->getSceneDenotation() == scene_denotation)
		{
			return scn;
		}
	}
	std::cout << "ERROR: Scene* ExtendedRenderWindow::getSceneFromDenotation(SCENE_DENOTATION scene_denotation), no scene exists with that denotation" << std::endl;
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
	//TEST-GAME
	if (this->window_state)
	{
		// clear the window with transparent(black) color
		this->clear(sf::Color::Transparent);

		if (active_scene != nullptr)
		{
			//Collision detection.
			this->active_scene->updateSceneFrame();
			//Updates the sf::View position.
			this->setView(*(active_scene->getCamera()->getCameraView()));
			//All layers are drawn.
			this->drawLayers();	
		}

		// Debugging.
		if (this->debugger_mode)
			this->debugDraw();

		//DEBUGGING
		this->drawLayouts(this->layouts[0]);
		//this->drawLayoutsDEBUG(this->layouts[0]);
		//this->drawButton((Button*)this->layouts[0]->getObjects()[0]);
		//END DEBUGGING
		
		// end the current frame. Display all changes
		this->display();
	}
	//PAUSE
	else
	{
		

		if (active_scene == nullptr) return;
		
		//DEBUGGING
		//!!!!!!!FIX THIS--shouldnt load each time!!!!!!!!!!!!!
		sf::Text text;
		sf::Font font;
		font.loadFromFile("../Project_AJJ/assets/ayar_takhu.ttf");
		text.setString("PAUSED");
		text.setFont(font);
		text.setCharacterSize(this->getSize().x / 16); // in pixels, not points!
		text.setFillColor(sf::Color::White);
		//-----------------------------------------------------

		sf::FloatRect cam_rect = this->getActiveScene()->getCamera()->getCameraViewRect();
		text.setPosition({ cam_rect.left + (cam_rect.width / 2) - (text.getLocalBounds().width / 2),
						   cam_rect.top + (cam_rect.height / 2) - (text.getLocalBounds().height / 2) });
		//pause-text
		this->draw(text);

		// end the current frame. Display all changes
		this->display();
		//END DEBUGGING
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
	std::vector<SceneLayer*>& layers = this->active_scene->getSceneLayers(); //For readability.
	// Get the layer-manipulated views of Camera.view.
	std::vector<sf::View> layer_views = this->active_scene->transformCameraViewToLayers();
	for (int i = 0; i < this->scene_layer_textures.size(); i++)
	{
		// Clears each RenderTexture in preparation for rendering.
		this->scene_layer_textures[i]->clear(sf::Color::Transparent);
		// Set RenderTexture view to that of the corresponding layer.
		this->scene_layer_textures[i]->setView(layer_views[i]);
		// Calculates the Drawable and positioning of each object WITHIN the view_rect corresponding
		// to the manipulated Camera view.
		for (Object* o : this->active_scene->getObjectsWithinCamera(layers[i]->layer_num))
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
				// 
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

void ExtendedRenderWindow::drawButton(Button * button)
{
	//Extract sprite from region
	sf::Sprite button_sprite = this->texture_manager->getAtlas(button->getTextureName())->getSprite(button->getFrame().texture_id, button->getFrame().region_name, button->getFrame().frame_index);

	//Set position and correct its scaling
	button_sprite.setPosition(button->getWorldPosition());
	button_sprite.setScale(button->getSize().x/ button_sprite.getLocalBounds().width, button->getSize().y / button_sprite.getLocalBounds().height);

	//Draw the button sprite
	this->draw(button_sprite);
	
	//Draw the button text on top
	this->draw(button->getText());
}

void ExtendedRenderWindow::drawLayouts(Layout* parent_layout)
{
	
	//only print the base-layout. Prevents double-draw
	if (parent_layout->getParentLayout() == nullptr)
	{	
		//check if we need to adjust its position depending on the camera view
		if (parent_layout->getfixedToView() == true)
		{
			//DEBUGGING
			sf::FloatRect cam_rect = this->getActiveScene()->getCamera()->getCameraViewRect();
			parent_layout->setPosition({ cam_rect.left + (cam_rect.width / 2) - (parent_layout->getRect().width / 2),
							   cam_rect.top + (cam_rect.height / 2) - (parent_layout->getRect().height / 2) });
			//END DEBUGGING
		}

		//draw the base-layout
		//this will only occur for the layout without a parent
		drawLayout(parent_layout);
	}
	
	//loop all child-layouts
	for (auto& l : parent_layout->getLayouts())
	{
		//draw the child-layout
		drawLayout(l);

		//recursion(draw the child-layout's children, grandchildren etc.)
		this->drawLayouts(l);
	}
}

void ExtendedRenderWindow::drawLayout(Layout* layout)
{
	sf::Sprite layout_sprite;
	float bsw = layout->getMinSize().x / 2;
	float bsh = layout->getMinSize().y / 2;

	sf::Vector2f l_pos = layout->getPosition();
	sf::Vector2f l_size = layout->getSize();
	int num_rows = this->texture_manager->getAtlas(layout->getTextureName())->getNumRows();
	int num_columns = this->texture_manager->getAtlas(layout->getTextureName())->getNumColumns();

	sf::Vector2f set_size;
	sf::Vector2f set_pos;
	

	for (int i = 0; i < num_rows; i++)
	{
		for (int j = 0; j < num_columns; j++)
		{
			set_size = { bsw,bsh };
			set_pos = l_pos;
			if (set_size.x > l_size.x) set_size.x = l_size.x;
			if (set_size.y > l_size.y) set_size.y = l_size.y;

			// Get the sprite
			layout_sprite = this->texture_manager->getAtlas(layout->getTextureName())->getSprite(TEXTURE_ID::IDLE, i * num_columns + j);
			

			//check if last row
			if (i == num_rows - 1)
			{
				set_pos.y = l_pos.y + l_size.y - bsh;
			}
			else if (i > 0)
			{
				set_size.y = (l_size.y - (2 * bsh)) / (num_rows - 2);
				set_pos.y = l_pos.y + bsh + (i - 1) * set_size.y;

			}

			//check if last column
			if (j == num_columns - 1)
			{
				set_pos.x = l_pos.x + l_size.x - bsw;
			}
			else if (j > 0)
			{
				set_size.x = (l_size.x - (2 * bsw)) / (num_columns - 2);
				set_pos.x = l_pos.x + bsw + (j - 1) * set_size.x;
			}


			//Set position
			layout_sprite.setPosition(set_pos);

			//Correct the layout_sprite's scaling
			layout_sprite.setScale(set_size.x / layout_sprite.getLocalBounds().width, set_size.y / layout_sprite.getLocalBounds().height);

			//Draw the sprite
			this->draw(layout_sprite);
		}
	}
}

void ExtendedRenderWindow::drawLayoutsDEBUG(Layout* parent_layout)
{
	//DEBUGGING simple test-rect
	sf::RectangleShape layout_rect_shape;
	layout_rect_shape.setSize(parent_layout->getSize());
	layout_rect_shape.setOutlineColor(sf::Color::Yellow);
	layout_rect_shape.setOutlineThickness(3);
	layout_rect_shape.setFillColor(sf::Color::Transparent);
	layout_rect_shape.setPosition(parent_layout->getPosition());

	//draw simple test-rect
	this->draw(layout_rect_shape);
	int index = 0;
	//loop all layouts
	for (auto& l : parent_layout->getLayouts())
	{
		//DEBUGGING simple test-rect
		layout_rect_shape.setSize(l->getSize());
		layout_rect_shape.setOutlineColor(sf::Color::Red);
		layout_rect_shape.setOutlineThickness(5);
		layout_rect_shape.setPosition(l->getPosition());

		//draw simple test-rect
		this->draw(layout_rect_shape);

		layout_rect_shape.setOutlineThickness(2);
		layout_rect_shape.setOutlineColor(sf::Color::Green);

		layout_rect_shape.setSize({ parent_layout->getMarginSpaces()[0].width, parent_layout->getMarginSpaces()[0].height });
		layout_rect_shape.setPosition({ parent_layout->getMarginSpaces()[0].left, parent_layout->getMarginSpaces()[0].top });
		this->draw(layout_rect_shape);

		layout_rect_shape.setOutlineColor(sf::Color::Blue);

		layout_rect_shape.setSize({ parent_layout->getMarginSpaces()[1].width, parent_layout->getMarginSpaces()[1].height });
		layout_rect_shape.setPosition({ parent_layout->getMarginSpaces()[1].left, parent_layout->getMarginSpaces()[1].top });
		this->draw(layout_rect_shape);

		layout_rect_shape.setOutlineColor(sf::Color::Cyan);

		layout_rect_shape.setSize({ parent_layout->getMarginSpaces()[2].width, parent_layout->getMarginSpaces()[2].height });
		layout_rect_shape.setPosition({ parent_layout->getMarginSpaces()[2].left, parent_layout->getMarginSpaces()[2].top });
		this->draw(layout_rect_shape);

		layout_rect_shape.setOutlineColor(sf::Color::Magenta);

		layout_rect_shape.setSize({ parent_layout->getMarginSpaces()[3].width, parent_layout->getMarginSpaces()[3].height });
		layout_rect_shape.setPosition({ parent_layout->getMarginSpaces()[3].left, parent_layout->getMarginSpaces()[3].top });
		this->draw(layout_rect_shape);

		//recursion
		//THIS STEP FAILS. NEED TO PASS RENDERTARGET https://stackoverflow.com/questions/54719752/c-sfml-printing-convex-shapes-to-the-screen-using-two-recursive-calls-only 
		//AND MAYBE THE RECT TO BE DRAWN??
		this->drawLayouts(l);
		//if (index == 1) { break; }
		index++;
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

/*void ExtendedRenderWindow::drawLayouts(Layout* parent_layout)
{
	sf::Sprite layout_sprite;

	//only print the base-layout. Prevents double-draw
	if (parent_layout->getParentLayout() == nullptr)
	{

		layout_sprite = this->texture_manager->getAtlas(parent_layout->getTextureName())->
						getSprite(parent_layout->getFrame().texture_id, parent_layout->getFrame().region_name, parent_layout->getFrame().frame_index);
		//Set position and correct its scaling
		layout_sprite.setPosition(parent_layout->getPosition());
		layout_sprite.setScale(parent_layout->getSize().x / layout_sprite.getLocalBounds().width, parent_layout->getSize().y / layout_sprite.getLocalBounds().height);

		this->draw(layout_sprite);
	}
	
	//loop all child-layouts
	for (auto& l : parent_layout->getLayouts())
	{
		layout_sprite = this->texture_manager->getAtlas(l->getTextureName())->
						getSprite(l->getFrame().texture_id, l->getFrame().region_name, l->getFrame().frame_index);		
		//Set position and correct its scaling
		layout_sprite.setPosition(l->getPosition());
		layout_sprite.setScale(l->getSize().x / layout_sprite.getLocalBounds().width, l->getSize().y / layout_sprite.getLocalBounds().height);

		this->draw(layout_sprite);

		//recursion
		this->drawLayouts(l);
	}
}*/