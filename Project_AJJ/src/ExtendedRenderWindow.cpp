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
	return nullptr;
}

//Setters
void ExtendedRenderWindow::setActiveScene(Scene* scene)
{
	active_scene = scene;
	this->clearSceneLayerTextures();
	int num_layers = static_cast<int>(active_scene->getSceneLayers().size());
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

//Others
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
		
		//check if we have any buttons
		if (this->layouts[0]->getNumButtons() > 0)
		{
			//draw a button(test)
			this->drawButton(this->layouts[0]->getObjects()[0]);
		}
			
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

void ExtendedRenderWindow::drawButton(Object* btn)
{
	//DEBUGGING
	//Extract sprite from region
	Button* button = (Button*)btn;
	/*
	sf::RectangleShape rect(button->getSize());
	rect.setPosition(button->getWorldPosition());*/
	sf::Sprite btn_sprite;
	btn_sprite = this->texture_manager->getAtlas(button->getTextureName())->getSprite(button->getCurrentFrame().texture_id, button->getCurrentFrame().region_name, button->getCurrentFrame().frame_index);
	btn_sprite.setPosition(button->getWorldPosition());
	btn_sprite.setScale(button->getSize().x / btn_sprite.getLocalBounds().width, button->getSize().y / btn_sprite.getLocalBounds().height);
	this->draw(btn_sprite);

	/*
	TextureAtlas* atlas = this->texture_manager->getAtlas(btn->getTextureName());
	sf::Sprite button_sprite = atlas->getSprite(btn->getCurrentFrame().texture_id, btn->getCurrentFrame().region_name, btn->getCurrentFrame().frame_index);

	//Set position and correct its scaling
	button_sprite.setPosition(btn->getWorldPosition());
	button_sprite.setScale(btn->getSize().x/ button_sprite.getLocalBounds().width, btn->getSize().y / button_sprite.getLocalBounds().height);

	//Draw the button sprite
	this->draw(button_sprite);
	*/

	//END DEBUGGING
}

void ExtendedRenderWindow::drawObject(Object* obj)
{

}

void ExtendedRenderWindow::drawLayouts(Layout* parent_layout)
{
	//check if we need to adjust its position depending on the camera view
	if (parent_layout->getfixedToView() == true)
	{
		//DEBUGGING
		//Currently it fixates the layout to the center of the screen
		sf::FloatRect cam_rect = this->getActiveScene()->getCamera()->getCameraViewRect();
		parent_layout->setPositionForAll({ cam_rect.left + (cam_rect.width / 2) - (parent_layout->getRect().width / 2),
							cam_rect.top + (cam_rect.height / 2) - (parent_layout->getRect().height / 2) });
		//END DEBUGGING
	}

	//draw the layout
	drawLayout(parent_layout);

	//loop all child-layouts
	for (auto& l : parent_layout->getLayouts())
	{
		//recursion(draw the child-layout's children, grandchildren etc.)
		this->drawLayouts(l);
	}
}

void ExtendedRenderWindow::drawLayout(Layout* layout)
{
	sf::Sprite layout_sprite;
	float bsw = layout->getBorderSize().x / 2;														//border set width
	float bsh = layout->getBorderSize().y / 2;														//border set height

	sf::Vector2f l_pos = layout->getPosition();														//layouts position
	sf::Vector2f l_size = layout->getSize();														//layouts size
	int num_rows = this->texture_manager->getAtlas(layout->getTextureName())->getNumRows();			//the number of rows in the layouts atlas
	int num_columns = this->texture_manager->getAtlas(layout->getTextureName())->getNumColumns();	//the number of columns in the layoputs atlas

	sf::Vector2f set_size;																			//the sprites size to be set at the end
	sf::Vector2f set_pos;																			//the sprites position to be set at the end

	//loop all of the rows in the atlas
	for (int i = 0; i < num_rows; i++)
	{
		//loop all of the columns in the atlas
		for (int j = 0; j < num_columns; j++)
		{
			//reset the set_size to the border size att each iteration
			//also, set the set_pos to the layouts position at each iteration
			set_size = { bsw,bsh };
			set_pos = l_pos;

			//get the sprite
			layout_sprite = this->texture_manager->getAtlas(layout->getTextureName())->getSprite(TEXTURE_ID::IDLE, i * num_columns + j);

			/*NOTE:
			Below we dont check for the first row. This is because
			the default y-position and height that is set above will be used.
			We also dont check for the first column. This is because
			the default x-position and width that is set above will be used.*/

			//check if last row
			if (i == num_rows - 1)
			{
				set_pos.y = l_pos.y + l_size.y - bsh;
			}
			//check if any other row, except for the first one
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


			//set position
			layout_sprite.setPosition(set_pos);

			//correct the layout_sprite's scaling
			layout_sprite.setScale(set_size.x / layout_sprite.getLocalBounds().width, set_size.y / layout_sprite.getLocalBounds().height);

			//draw the sprite
			this->draw(layout_sprite);
		}
	}
}

bool ExtendedRenderWindow::cursorHover(sf::FloatRect rect)
{
	//get the cursors position relative to the window
	sf::Vector2i cursor_pos = sf::Mouse::getPosition(*this);

	//map the coordinates to the current view
	sf::Vector2f real_world_pos = this->mapPixelToCoords(cursor_pos);

	//return true or false depending on if the cursor is contained in the rect
	return (rect.contains(real_world_pos) ? true : false);
}

std::vector<sf::FloatRect> ExtendedRenderWindow::cursorHoverRects(std::vector<sf::FloatRect> rects)
{
	std::vector<sf::FloatRect> hovered_rects;		//the rects that the mouse is hovering

	//loop all of the input rects
	for (auto& rect : rects)
	{
		//if the cursor hovers the rect
		if (cursorHover(rect))
		{
			//push it to the vector to be returned
			hovered_rects.push_back(rect);
		}
	}

	//at last, return all of the hovered rects
	return hovered_rects;
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
			sf::Sprite obj_sprite = obj_atlas->getSprite(o->getCurrentFrame().texture_id, o->getCurrentFrame().region_name, o->getCurrentFrame().frame_index);
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
			layer_sprite.scale(sf::Vector2f({ zoom_factor, zoom_factor }));

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

