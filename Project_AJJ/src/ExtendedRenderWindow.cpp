#include "ExtendedRenderWindow.h"

unsigned int ExtendedRenderWindow::instance_counter = 0;

ExtendedRenderWindow::ExtendedRenderWindow(sf::Vector2u resolution, std::string window_title, sf::ContextSettings settings)
	: sf::RenderWindow(sf::VideoMode(resolution.x, resolution.y), window_title, sf::Style::Default, settings)
{
	ExtendedRenderWindow::instance_counter++;
}

ExtendedRenderWindow::~ExtendedRenderWindow() { ExtendedRenderWindow::instance_counter--; }

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
	if (active_scene != nullptr && this->window_state)
	{
		//Clear the window with transparent(black) color
		this->clear(sf::Color::Transparent);

		//Collision detection.
		this->active_scene->updateSceneFrame();

		//Updates the camera-view
		this->setView(*(active_scene->getCamera()->getCameraView()));
	}

	//All layers are drawn.
	//by placing it outside the above if-statement, only the main-layer will be paused
	this->drawLayers();

	//DEBUGGING
	if (this->debugger_mode) this->debugDraw();	
	//END DEBUGGING
	
	//End the current frame. Display all changes
	this->display();
}

void ExtendedRenderWindow::drawActiveSceneToWindow()
{
	if (this->window_state)
	{
		// clear the window with transparent(black) color
		this->clear(sf::Color::Transparent);

		if (active_scene != nullptr)
		{
			//Collision detection.
			this->active_scene->updateSceneFrame();
			//All layers are drawn.
			this->drawLayersToWindow();
		}

		// Debugging.
		if (this->debugger_mode)
			this->debugDraw();

		// end the current frame. Display all changes
		this->display();
	}
}

void ExtendedRenderWindow::drawLayoutObject(Object* obj, sf::RenderTexture* render_texture)
{
	//BUTTON
	if (obj->getClassId() == "button")
	{
		//cast it as a Button *
		Button* button = (Button*)obj;

		//get the sprite associated with the button
		sf::Sprite btn_sprite;

		//get the current frame in use for the button
		Frame btn_cur_frm = button->getCurrentFrame();

		btn_sprite = this->texture_manager->getAtlas(button->getTextureName())->getSprite(btn_cur_frm.texture_id, btn_cur_frm.region_name, btn_cur_frm.frame_index);
		
		//adjust its position and scaling to fit inside of the object
		btn_sprite.setPosition({ button->getWorldPosition().x ,button->getWorldPosition().y });
		btn_sprite.setScale(button->getSize().x / btn_sprite.getLocalBounds().width, button->getSize().y / btn_sprite.getLocalBounds().height);

		//at last, draw the sprite
		render_texture->draw(btn_sprite);

		//center the text
		button->centerText();

		//draw the text
		render_texture->draw(button->getText());
	}
	//DEFAULT OBJECT
	else if (obj->getClassId() == "default")
	{

	}
}

void ExtendedRenderWindow::drawLayoutObjectToWindow(Object* obj)
{
	//BUTTON
	if (obj->getClassId() == "button")
	{
		//cast it as a Button *
		Button* button = (Button*)obj;

		//get the sprite associated with the button
		sf::Sprite btn_sprite;

		//get the current frame in use for the button
		Frame btn_cur_frm = button->getCurrentFrame();

		btn_sprite = this->texture_manager->getAtlas(button->getTextureName())->getSprite(btn_cur_frm.texture_id, btn_cur_frm.region_name, btn_cur_frm.frame_index);

		//adjust its position and scaling to fit inside of the object
		btn_sprite.setPosition({ button->getWorldPosition().x ,button->getWorldPosition().y });
		btn_sprite.setScale(button->getSize().x / btn_sprite.getLocalBounds().width, button->getSize().y / btn_sprite.getLocalBounds().height);

		//at last, draw the sprite
		this->draw(btn_sprite);

		//center the text
		button->centerText();

		//draw the text
		this->draw(button->getText());
	}
	//DEFAULT OBJECT
	else if (obj->getClassId() == "default")
	{

	}
}

void ExtendedRenderWindow::drawObject(Object* obj)
{

}

void ExtendedRenderWindow::drawLayouts(Layout* parent_layout, sf::RenderTexture * render_texture)
{
	//check if we need to adjust its position depending on the camera view
	if (parent_layout->getfixedToView() == true)
	{
		//get the viewRect of the camera
		sf::FloatRect cam_rect = this->getActiveScene()->getCamera()->getCameraViewRect();
		//update the position for the parent_layout and all of its children
		//the parent_layout will be centered and everything else will be adjusted accordingly
		parent_layout->setPositionForAll({ cam_rect.left + (cam_rect.width / 2) - (parent_layout->getRect().width / 2),
										   cam_rect.top + (cam_rect.height / 2) - (parent_layout->getRect().height / 2) });
	}

	//draw the layout if its enabled
	if(parent_layout->getEnabled())
		drawLayout(parent_layout, render_texture);

	//check if we have any objects in the layout
	for(auto& layout_obj : parent_layout->getObjects())
	{
		//draw a button
		this->drawLayoutObject(layout_obj, render_texture);
	}

	//loop all child-layouts
	for (auto& child_layout : parent_layout->getLayouts())
	{
		//recursion(draw the child-layout's children, grandchildren etc.)
		this->drawLayouts(child_layout, render_texture);
	}
}

void ExtendedRenderWindow::drawLayoutsToWindow(Layout* parent_layout)
{
	//check if we need to adjust its position depending on the camera view
	if (parent_layout->getfixedToView() == true)
	{
		//get the viewRect of the camera
		sf::FloatRect cam_rect = this->getActiveScene()->getCamera()->getCameraViewRect();
		//update the position for the parent_layout and all of its children
		//the parent_layout will be centered and everything else will be adjusted accordingly
		parent_layout->setPositionForAll({ cam_rect.left + (cam_rect.width / 2) - (parent_layout->getRect().width / 2),
										   cam_rect.top + (cam_rect.height / 2) - (parent_layout->getRect().height / 2) });
	}

	//draw the layout if its enabled
	if (parent_layout->getEnabled())
		drawLayoutToWindow(parent_layout);

	//check if we have any objects in the layout
	for (auto& layout_obj : parent_layout->getObjects())
	{
		//draw a button
		this->drawLayoutObjectToWindow(layout_obj);
	}

	//loop all child-layouts
	for (auto& child_layout : parent_layout->getLayouts())
	{
		//recursion(draw the child-layout's children, grandchildren etc.)
		this->drawLayoutsToWindow(child_layout);
	}
}

void ExtendedRenderWindow::drawLayout(Layout* layout, sf::RenderTexture * render_texture)
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
			render_texture->draw(layout_sprite);

		}
	}
}

void ExtendedRenderWindow::drawLayoutToWindow(Layout* layout)
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


//Private functions.

void ExtendedRenderWindow::clearSceneLayerTextures()
{
	for (sf::RenderTexture* t : this->scene_layer_textures)
		delete t;
	this->scene_layer_textures.resize(0);
}

void ExtendedRenderWindow::drawLayers()
{
	std::vector<int> layer_nums = this->active_scene->getLayerNums();

	// Get the layer-manipulated views of Camera.view.
	std::vector<sf::View> layer_views = this->active_scene->getLayerManipulatedViews();

	for (int i = 0; i < this->scene_layer_textures.size(); i++)
	{
		// Clears each RenderTexture in preparation for rendering.
		this->scene_layer_textures[i]->clear(sf::Color::Transparent);

		//DEBUGGING
		//IF STATIC LAYER
		if (layer_nums[i] == -3)
		{
			layer_views[i].setCenter(this->getSize().x / 2, this->getSize().y / 2);
		}
		//END DEBUGGING
		
		// Set RenderTexture view to that of the corresponding layer.
		this->scene_layer_textures[i]->setView(layer_views[i]);

		// Calculates the Drawable and positioning of each object WITHIN the view_rect corresponding
		// to the manipulated Camera view.
		for (Object* o : this->active_scene->getObjectsWithinCamera(layer_nums[i]))
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
		
			if (layer_nums[i] == 0)
			{
				//Draws object directly onto RenderWindow if main scene layer.

				/*
				//DEBUGGING
				this->draw(obj_gs);
				this->draw(o->vel_vec[0]);
				this->draw(o->vel_vec[1]);
				this->draw(o->vel_vec[2]);
				this->draw(o->vel_vec[3]);
				//END DEBUGGING
				*/

				this->draw(obj_sprite);
			}
			else
			{
				//Draws object to RenderTexture if any other layer.
				this->scene_layer_textures[i]->draw(obj_sprite);		
			}
		}

		for (Layout* l : this->active_scene->getLayer(layer_nums[i])->getLayouts())
		{
			this->drawLayouts(l, this->scene_layer_textures[i]);
		}
		
		//Converts all layer textures (except main scene layer (0) ) to sprites and draws them onto RenderWindow.
		if (layer_nums[i] != 0)
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

void ExtendedRenderWindow::drawLayersToWindow()
{
	std::vector<int> layer_nums = this->active_scene->getLayerNums();
	// Get the layer-manipulated views of Camera.view.
	std::vector<sf::View> layer_views = this->active_scene->getLayerManipulatedViews();
	for (int i = 0; i < layer_nums.size(); i++)
	{
		this->setView(layer_views[i]);
		this->drawLayerToWindow(layer_nums[i]);
	}
}

void ExtendedRenderWindow::drawLayerToWindow(int layer_num)
{
	// Calculates the Drawable and positioning of each object WITHIN the view_rect corresponding
	// to the manipulated Camera view.
	for (Object* o : this->active_scene->getObjectsWithinCamera(layer_num))
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
		this->draw(obj_sprite);
	}
	
	for (Layout* l : this->active_scene->getLayer(layer_num)->getLayouts())
	{
		l->resetMarginSpaces();
		l->placeLayouts();
		this->drawLayoutsToWindow(l);
	}
}


// Debugging.
void ExtendedRenderWindow::debugDraw()
{
	/*
	// COLLISIONDETECTION GRID LINES
	if (this->active_scene != nullptr)
	{
		std::vector<sf::VertexArray> grid = this->active_scene->getCollisionDetection()->getGrid();
		for (int i = 0; i < grid.size(); i++)
		{
			this->draw(grid[i]);
		}
	}
	*/
	
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

	// HITBOXNODES.
	if (this->active_scene != nullptr)
	{
		std::vector<HitboxNode*> hitboxes = this->active_scene->getCollisionDetection()->getHitboxes();
		this->setView(*this->active_scene->getCamera()->getCameraView());
		for (int i = 0; i < hitboxes.size(); i++)
		{
			this->draw(hitboxes[i]->getDrawable());
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
		//this->drawLayouts(l);
		//if (index == 1) { break; }
		index++;
	}
}

