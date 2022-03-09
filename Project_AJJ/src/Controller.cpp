#include "Controller.h"

unsigned int Controller::instance_counter = 0;

Controller::Controller()
{
	Controller::instance_counter++;
}

Controller::~Controller()
{
	Controller::instance_counter--;
}

/*Setters*/
void Controller::setWindow(ExtendedRenderWindow* window)
{
	this->window = window;
	this->original_view_size = this->window->getActiveScene()->getCamera()->getCameraView()->getSize();
}

void Controller::setObject(Object* obj)
{
	this->obj = obj;
}

void Controller::setMouseButtonState(sf::Mouse::Button mouse_btn, bool state) 
{
	//pressed <-> state = true
	//not pressed <-> state = false

	if (mouse_btn == sf::Mouse::Button::Left)
	{
		this->left_mouse_button_state = state;
	}
	else if (mouse_btn == sf::Mouse::Button::Right)
	{
		this->right_mouse_button_state = state;
	}
}

/*Getters*/
bool Controller::getMouseButtonState(sf::Mouse::Button mouse_btn)
{
	if (mouse_btn == sf::Mouse::Button::Left)
	{
		return this->left_mouse_button_state;
	}
	else if (mouse_btn == sf::Mouse::Button::Right)
	{
		return this->right_mouse_button_state;
	}
}


/*Bind action to input*/
void Controller::bindActionToKey(Action* action, sf::Keyboard::Key key)
{
	ActionNode new_actionnode;
	new_actionnode.action = action;
	new_actionnode.keys.push_back(key);
	new_actionnode.id = this->num_actionnodes;
	this->num_actionnodes++;
	this->actionnodes.push_back(new_actionnode);
}

void Controller::bindActionToKey(Action* action, std::vector<sf::Keyboard::Key> keys)
{
	ActionNode new_actionnode;
	new_actionnode.action = action;
	new_actionnode.keys = keys;
	new_actionnode.id = this->num_actionnodes;
	this->num_actionnodes++;
	this->actionnodes.push_back(new_actionnode);
}

void Controller::bindActionToMouseButton(Action* action, sf::Mouse::Button button)
{
	ActionNode new_actionnode;
	new_actionnode.action = action;
	new_actionnode.mouse_button = button;
	new_actionnode.id = this->num_actionnodes;
	this->num_actionnodes++;
	this->actionnodes.push_back(new_actionnode);
}

void Controller::bindActionToMouseButton(Action* action, Object* obj, sf::Mouse::Button button)
{
	ActionNode new_actionkey;
	new_actionkey.action = action;
	new_actionkey.mouse_button = button;
	new_actionkey.obj = obj;
	new_actionkey.id = this->num_actionnodes;
	this->num_actionnodes++;
	this->actionnodes.push_back(new_actionkey);
}

/*Cursor related*/
bool Controller::cursorHover(sf::FloatRect rect, bool on_fixated_layer)
{
	//get the cursors position relative to the window
	sf::Vector2i cursor_pos = sf::Mouse::getPosition(*this->window);
	sf::Vector2f point = (sf::Vector2f)cursor_pos;

	//map the coordinates to the current view
	if (!on_fixated_layer)
	{
		//map cursor-position to real-world coordinates
		point = this->window->mapPixelToCoords(cursor_pos);
	}
	
	//std::cout << "real_world_pos: " << cursor_pos.x << ", " << cursor_pos.y << std::endl;
	//return true or false depending on if the cursor is contained in the rect
	return (rect.contains(point) ? true : false);
}

std::vector<sf::FloatRect> Controller::cursorHoverRects(std::vector<sf::FloatRect> rects)
{
	std::vector<sf::FloatRect> hovered_rects;		//the rects that the mouse is hovering

	//loop all of the input rects
	for (auto& rect : rects)
	{
		//if the cursor hovers the rect
		if (cursorHover(rect,true))
		{
			//push it to the vector to be returned
			hovered_rects.push_back(rect);
		}
	}

	//at last, return all of the hovered rects
	return hovered_rects;
}

/*Main methods for controlling an object*/
std::vector<ActionNode> Controller::constructActiveActions()
{

	
	//for-loop for constructing the active_actionnodes vector
	for (auto& a_node : this->actionnodes)
	{
		//assumes action state.
		bool action_state = true;		

		/*******************************************************************
		 *-----------------Check if assumption is true---------------------*
		 *******************************************************************/
		
		//check if we have an object to relate the action to
		if (a_node.obj != nullptr)
		{	
			//BUTTONS
			if (a_node.obj->getClassId() == "button")
			{
				//cast as a button
				Button* btn = (Button*)a_node.obj;

				bool btn_hovered = cursorHover(btn->getFloatRect(), btn->getOnFixatedLayer());													//hovered = cursor currently over btn-rect
				bool btn_pressed = sf::Mouse::isButtonPressed(a_node.mouse_button) && btn_hovered;												//pressed = hovered + mouse-button pressed
				bool btn_released = !sf::Mouse::isButtonPressed(a_node.mouse_button) && getMouseButtonState(a_node.mouse_button) && btn_hovered;//released = mouse-button not pressed + we are coming out of a pressed state + hovered
				bool btn_clicked = btn->getFloatRect().contains((sf::Vector2f)this->last_cursor_press_pos) && btn_released;						//clicked = the last cursor-press happened on the button-rect + released i.e. pressed + released

				//if we arent on a fixated layer, the last_cursor_press_pos need to be adjusted from pixel to coords. We therefore need to change btn_clicked
				if (!btn->getOnFixatedLayer())
				{
					btn_clicked = btn->getFloatRect().contains(this->window->mapPixelToCoords(this->last_cursor_press_pos)) && btn_released;
				}	
					
				TRIGGER_TYPE btn_trigger = btn->getTriggerType();															//the trigger-type of this button i.e. what type of input that triggers it

				//check if assumption is true or not depending on the trigger-type
				switch (btn_trigger) 
				{
				case TRIGGER_TYPE::CLICKED:
					if (!btn_clicked)
					{
						action_state = false;
					}
					break;

				case TRIGGER_TYPE::PRESSED:
					if (!btn_pressed)
					{
						action_state = false;
					}
					break;

				case TRIGGER_TYPE::HOVERED:
					if (!btn_hovered)
					{
						action_state = false;
					}
					break;

				case TRIGGER_TYPE::RELEASED:
					if (!btn_released)
					{
						action_state = false;
					}
					break;

				case TRIGGER_TYPE::PRESSED_HOVER:

					break;

				default:
					std::cout << "ERROR: std::vector<ActionNode> Controller::constructActiveActions(), faulty trigger-type." << std::endl;
				}

				//update the frame depending on hovered/pressed/default
				btn->updateFrame(btn_pressed, btn_hovered);
			}

			//SCROLLBAR

			//SLIDER

			//ETC
		}		

		for (int j = 0; j < a_node.keys.size(); j++)
		{
			//If one of the binded action keys is not pressed, 
			if (!sf::Keyboard::isKeyPressed(a_node.keys[j]))
			{
				//the assumption is wrong.
				action_state = false;								
			}
		}

		/*******************************************************************
		 *------------Add or remove from active_actionnodes----------------*
		 *******************************************************************/

		//if the action_state is true(1) for this specific action_key
		if (action_state)
		{
			//DEBUGGING
			//std::cout << a_node.action->getActionName() << std::endl;
			//END DEBUGGING

			//check if we want to add the actions to the active_action vector
			int add_to_active_actions = 1;
			for (auto& active_a_node : active_actionnodes)
			{
				if (sameActionnode(active_a_node, a_node))
				{
					add_to_active_actions = 0;					//if its already in the list, set to false(0)
					break;
				}

			}

			//if its not in the list already
			if (add_to_active_actions)
			{
				active_actionnodes.push_back(a_node);			//add to active action vector
				this->num_active_actionnodes++;
			}
		}
		//else, check if its in the list from previous addition. If so, remove it
		else
		{
			int k = 0;											//keeps track of index

			//loop all of the current active_actions
			for (auto& active_a_node : this->active_actionnodes)
			{
				//if the actionnodes action is in the active_actionnodes vector
				if (sameActionnode(active_a_node, a_node))
				{
					//remove it, since its key is not pressed
					this->active_actionnodes.erase(this->active_actionnodes.begin() + k);

					//reduce the number of active_actions
					this->num_active_actionnodes--;

					break;
				}
				k++;
			}
		}
	}
	//at last, return the active_actionnodes
	return this->active_actionnodes;
}

void Controller::triggerActiveActions()
{
	/*The active_action list is contructed so that the last element
		* in the vector is the action with the highest priority(called last).
		* Thats why we loop the vector backwards.
		* We also want to check so that functions that manipulate the same thing,
		* for example move left / right that both manipulate vertical movement,
		* doesnt clashand cancel eachother out.We want to prioritize one, which
		* is the one with the highest priority.
		* Variables hori_move / vert_move and perform_action is there to make sure
		* that this does not happern.
		*/
	
	//loop all active object actions

	int hori_move = 0;													//horizontal movement has been/not been made
	int vert_move = 0;													//vertical movement has been/not been made

	for (int i = this->num_active_actionnodes - 1; i >= 0; i--)
	{
		int perform_action = 1;											//assumes action state

		ACTIONTYPE type = this->active_actionnodes[i].action->getActionType();		//current actiontype

		//if we want to move horizontally
		if (type == ACTIONTYPE::MOVE_LEFT || type == ACTIONTYPE::MOVE_RIGHT)
		{
			//if we have moved horizontally already
			if (hori_move != 0)
			{
				//do not perform the action
				perform_action = 0;
			}
			//regardless, set horizontal movement to true
			hori_move = 1;
		}

		//if we want to move vertically
		else if (type == ACTIONTYPE::MOVE_UP || type == ACTIONTYPE::MOVE_DOWN)
		{
			//if we have moved vertically already
			if (vert_move != 0)
			{
				//do not perform the action.
				perform_action = 0;
			}
			//regardless, set vertical movement to true
			vert_move = 1;
		}
			
		//if the assumed action state remains true(1)
		if (perform_action)
		{
			this->active_actionnodes[i].action->triggerAction();
		}

	}

	//correct horizontal movement to not overstep velocity-ellipsiod
	correctMovement();
}

void Controller::correctMovement()
{
	sf::Vector2f obj_vel = this->obj->getVelocity();							//objects current velocity vector
	sf::Vector2f cor_vel = obj_vel;												//objects corrected velocity vector
	sf::Vector2f max_vel = this->obj->getMaxVelocity();							//objects maximum velocity 
	float len_vel = sqrt(obj_vel.x * obj_vel.x + obj_vel.y * obj_vel.y);		//length of object velocity vector
	const float PI = 3.1415927f;												//constant for PI

	//calculate the point on the velocity-ellipsiod from the angle of the objects velocity
	//compare the vector given from the objects center to that point with len_vel
	//if len_vel is greater, set len_vel to be equal to the newly calculated vector
	//this calculation does not need to be done if the velocity-ellipsiod is a circle
	//i.e. the maximum speed is the same for horizontal and vertical movements


	if ((max_vel.x != max_vel.y) && (len_vel > max_vel.x || len_vel > max_vel.y))
	{
		//calculate degree of travel (horizontally mirrored xy axis.)
		float deg = atan(obj_vel.y / obj_vel.x);

		//set the degree to positive. To simplify correction
		if (deg < 0)
		{
			deg = -deg;
		}

		//correct the degree of travel depending on direction
		if (obj_vel.x > 0)
		{
			if (obj_vel.y < 0)
			{
				deg = 2 * PI - deg;
			}
		}
		else if (obj_vel.x < 0)
		{
			if (obj_vel.y < 0)
			{
				deg = PI + deg;
			}
			else if (obj_vel.y > 0)
			{
				deg = PI - deg;
			}
			else
			{
				deg = PI;
			}
		}
		else
		{
			if (obj_vel.y > 0)
			{
				deg = PI / 2;
			}
			else if (obj_vel.y < 0)
			{
				deg = (3 * PI) / 2;
			}
		}

		//calculate x on the velocity-ellipsiod
		float a = max_vel.x;
		float b = max_vel.y;
		float x = (a * b) / sqrt((b * b) + (a * a) * (tan(deg) * tan(deg)));
		if (obj_vel.x < 0) { x = -x; }

		//calculate y from the ellipse-formula
		float y = sqrt((1 - ((x * x) / (a * a))) * (b * b));
		if (obj_vel.y < 0) { y = -y; }

		cor_vel = { x,y };
	}
	else if (len_vel > max_vel.x || len_vel > max_vel.y)
	{
		//normalize vector obj_vel and cap its length at the maximum stepsize/velocity
		cor_vel = { (obj_vel.x / len_vel) * max_vel.x,(obj_vel.y / len_vel) * max_vel.y };
	}

	//update the objects worldposition
	this->obj->setWorldPosition(this->obj->getWorldPosition() + cor_vel);
}

void Controller::processUserInput()
{		
	if (this->controller_state)
	{
		//construct the active_actions vector
		constructActiveActions();

		//if there are no active_actions
		if (active_actionnodes.empty())
		{
			active_actionnodes.push_back(actionnodes[0]);				//add the idle/not-active action
			this->num_active_actionnodes++;
		}

		//last, trigger all the actions that have been activated
		triggerActiveActions();
	}
}

/*Help functions*/
bool Controller::sameActionnode(ActionNode actionnode_1, ActionNode actionnode_2)
{
	//check the equality of the id's. If they are the same, return true.
	return (actionnode_1.id == actionnode_2.id ? true : false);
}
