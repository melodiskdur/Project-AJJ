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
	}
	//if the object is moving vertically and no vertical movementaction is triggered
	//we want to slow down!!
	if (set_vel.y != 0 && !vert_move)
	{
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
	}

	//set the velocity
	this->obj->setVelocity(set_vel);
}

std::vector<Action*> Controller::constructActiveActions()
{
	//for-loop for constructing the active_actions vector
	for (auto& action_key : this->action_keys)
	{
		bool action_state = true;									//Assumes action state.

		for (int j = 0; j < action_key.keys.size(); j++)
		{
			if (!sf::Keyboard::isKeyPressed(action_key.keys[j]))	//If one of the binded action keys is not pressed, 
			{
				action_state = false;								//the assumption is wrong.

				int k = 0;
				for (auto& active_action_element : active_actions)
				{
					if (active_action_element == action_key.action)
					{
						//if the action is in the active_actions vector, remove it
						active_actions.erase(active_actions.begin() + k);

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
			for (auto& active_action_element : active_actions)
			{
				if (active_action_element == action_key.action)
				{
					add_to_active_actions = 0;								//if its already in the list, set to false(0)
					break;
				}

			}

			//if its not in the list already
			if (add_to_active_actions)
			{
				active_actions.push_back(action_key.action);				//add to active action vector
				this->num_of_active_actions++;
			}


		}

	}

	return this->active_actions;
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
	
	int hori_move = 0;													//horizontal movement has been/not been made
	int vert_move = 0;													//vertical movement has been/not been made

	for (int i = this->num_of_active_actions - 1; i >= 0; i--)
	{
		int perform_action = 1;											//assumes action state

		ACTIONTYPE type = this->active_actions[i]->getActionType();		//current actiontype

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
				//do not perform the action
				perform_action = 0;
			}
			//regardless, set vertical movement to true
			vert_move = 1;
		}
			
		//if the assumed action state remains true(1)
		if (perform_action)
		{
			this->active_actions[i]->triggerAction();
		}

	}

	//break object movement in vert/hori direction if no actions were triggered in those directions
	breakMovement(hori_move , vert_move , 2 );
}

void Controller::processUserInput()
{		
	if (this->controller_state)
	{
		//construct the active_actions vector
		constructActiveActions();

		//if there are no active_actions
		if (active_actions.empty())
		{
			active_actions.push_back(action_keys[0].action);				//add the idle/not-active action
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
