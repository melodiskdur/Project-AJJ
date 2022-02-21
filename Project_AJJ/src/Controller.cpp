#include "Controller.h"

Controller::Controller()
{

}

Controller::~Controller()
{

}

void Controller::bindActionToKey(Action* action, sf::Keyboard::Key key)
{
	ActionKey new_actionkey;
	new_actionkey.action = action;
	new_actionkey.keys.push_back(key);
	this->action_keys.push_back(new_actionkey);
}

void Controller::bindActionToKey(Action* action, std::vector<sf::Keyboard::Key> keys)
{
	ActionKey new_actionkey;
	new_actionkey.action = action;
	new_actionkey.keys = keys;
	this->action_keys.push_back(new_actionkey);
}

void Controller::bindActionToMouseButton(Action* action, sf::Mouse::Button button)
{
	ActionKey new_actionkey;
	new_actionkey.action = action;
	new_actionkey.mouse_button = button;
	this->action_keys.push_back(new_actionkey);
}

void Controller::setWindow(ExtendedRenderWindow* window)
{
	this->window = window;
	this->original_view_size = this->window->getActiveScene()->getCamera()->getCameraView()->getSize();
}


void Controller::setObject(Object* obj)
{
	this->obj = obj;
}

void Controller::breakMovement(int hori_move, int vert_move, float break_value)
{
	//the set_velocity is the velocity that will be set
	sf::Vector2f set_vel = this->obj->getVelocity();


	//if the object is moving horizontally and no horizontal movementaction is triggered
	//we want to slow down!!
	if (set_vel.x != 0 && !hori_move)
	{
		set_vel.x = 0;
		/*
		if (set_vel.x < 0 && set_vel.x + break_value <= 0)
		{
			set_vel.x += break_value;
		}
		else if (set_vel.x < 0 && set_vel.x + break_value > 0)
		{
			set_vel.x = 0;
		}
		if (set_vel.x > 0 && set_vel.x - break_value >= 0)
		{
			set_vel.x -= break_value;
		}
		else if (set_vel.x > 0 && set_vel.x - break_value < 0)
		{
			set_vel.x = 0;
		}
		*/
	}
	//if the object is moving vertically and no vertical movementaction is triggered
	//we want to slow down!!
	if (set_vel.y != 0 && !vert_move)
	{
		set_vel.y = 0;
		/*
		if (set_vel.y < 0 && set_vel.y + break_value <= 0)
		{
			set_vel.y += break_value;
		}
		else if (set_vel.y < 0 && set_vel.y + break_value > 0)
		{
			set_vel.y = 0;
		}
		if (set_vel.y > 0 && set_vel.y - break_value >= 0)
		{
			set_vel.y -= break_value;
		}
		else if (set_vel.y > 0 && set_vel.y - break_value < 0)
		{
			set_vel.y = 0;
		}
		*/
	}

	//set the velocity
	this->obj->setVelocity(set_vel);
}

bool Controller::sameActionKey(ActionKey ak1, ActionKey ak2)
{
	//assume that they are the same
	bool action_match = true;
	bool keys_match = true;
	bool mouse_btn_match = true;

	//check if action match
	if (ak1.action != ak2.action) action_match = false;

	//check if keys match
	for (auto& k1 : ak1.keys)
	{
		for (auto& k2 : ak2.keys)
		{
			if (k1 != k2) keys_match = false;
		}
	}

	//check if mouse button match
	if (ak1.mouse_button != ak2.mouse_button) mouse_btn_match = false;

	//if the action match and if at least keys or mouse button match, return true.
	return (action_match && (keys_match || mouse_btn_match) ? true : false);
}

std::vector<ActionKey> Controller::constructActiveActions()
{
	//for-loop for constructing the active_actionkeys vector
	for (auto& action_key : this->action_keys)
	{
		//Assumes action state.
		bool action_state = true;		

		if (action_key.mouse_button == sf::Mouse::Button::Left)
		{
			if (!(sf::Mouse::isButtonPressed(action_key.mouse_button) && this->window->cursorHover(this->window->getLayouts()[0]->getButtons()[0]->getFloatRect())))
			{
				//the assumption is wrong.
				action_state = false;

				int k = 0;											//keeps track of index
				//loop all of the current active_actions
				for (auto& ak : this->active_actionkeys)
				{
					//if the action_keys action is in the active_actionkeys vector
					if (sameActionKey(ak, action_key))
					{
						//remove it, since its key is not pressed
						this->active_actionkeys.erase(this->active_actionkeys.begin() + k);

						//reduce the number of active_actions
						if (this->num_of_active_actions > 0)
							this->num_of_active_actions--;
					}
					k++;
				}
			}
		}
		

		for (int j = 0; j < action_key.keys.size(); j++)
		{
			//If one of the binded action keys is not pressed, 
			if (!sf::Keyboard::isKeyPressed(action_key.keys[j]))	
			{
				//the assumption is wrong.
				action_state = false;								

				int k = 0;											//keeps track of index
				//loop all of the current active_actions
				for (auto& ak : this->active_actionkeys)
				{
					//if the action_keys action is in the active_actionkeys vector
					if (sameActionKey(ak,action_key))
					{
						//remove it, since its key is not pressed
						this->active_actionkeys.erase(this->active_actionkeys.begin() + k);

						//reduce the number of active_actions
						if (this->num_of_active_actions > 0)
							this->num_of_active_actions--;
					}
					k++;
				}
				break;
			}
		}
		

		//if the action_state is true(1) for this specific action_key
		if (action_state)
		{
			//std::cout << action_keys[i].action->getActionName() << std::endl;

			//check if we want to add the actions to the active_action vector
			int add_to_active_actions = 1;
			for (auto& aak : active_actionkeys)
			{
				if (sameActionKey(aak, action_key))
				{
					add_to_active_actions = 0;								//if its already in the list, set to false(0)
					break;
				}

			}

			//if its not in the list already
			if (add_to_active_actions)
			{
				active_actionkeys.push_back(action_key);				//add to active action vector
				this->num_of_active_actions++;
			}


		}

	}

	return this->active_actionkeys;
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

	for (int i = this->num_of_active_actions - 1; i >= 0; i--)
	{
		int perform_action = 1;											//assumes action state

		ACTIONTYPE type = this->active_actionkeys[i].action->getActionType();		//current actiontype

		//if we want to move horizontally
		if (type == ACTIONTYPE::AT_MOVE_LEFT || type == ACTIONTYPE::AT_MOVE_RIGHT)
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
		else if (type == ACTIONTYPE::AT_MOVE_UP || type == ACTIONTYPE::AT_MOVE_DOWN)
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
			this->active_actionkeys[i].action->triggerAction();
		}

	}

	//break object movement in vert/hori direction if no actions were triggered in those directions
	//!!!!!!!!!!FIX THIS-only a fast solution. Should be implemented as a physics attribute ( friction or airresistance )!!!!!!!!!!!!!!!!
	//breakMovement(hori_move, vert_move, 2);

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
		if (active_actionkeys.empty())
		{
			active_actionkeys.push_back(action_keys[0]);				//add the idle/not-active action
			this->num_of_active_actions++;
		}

		//last, trigger all the actions that have been activated
		triggerActiveActions();
	}
	else
	{
		std::cout << "not active\n";
	}
}
