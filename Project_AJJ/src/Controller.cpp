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

void Controller::processUserInput()
{
	bool action_state;												//Assumes action state.
	bool action_triggered = false;
	for (int i = 0; i < action_keys.size(); i++)
	{
		action_state = true;

		for (int j = 0; j < action_keys[i].keys.size(); j++)
		{
			if (!sf::Keyboard::isKeyPressed(action_keys[i].keys[j])) //If one of the binded action keys is not pressed, 
			{												         //the assumption is wrong.
				action_state = false;
				break;
			}
		}

		if (action_state)
		{
			std::cout << action_keys[i].action->getActionName() << std::endl;
			action_keys[i].action->triggerAction();					//If the action state remained true, action is triggered.
			action_triggered = true;								//Set action_triggered to true
		}	
	}
	if (!action_triggered)									//if the action_state is false and any previous actions were'nt triggered
	{
		//MAKE THIS A SEPERATE FUNKTION CALLED update. Change the current update functions name to ex gotonextframe.
		this->action_keys[0].action->getParentObject()->setFrame(this->action_keys[0].action->getAnimation()->getActiveFrame());
		this->action_keys[0].action->getAnimation()->update();
		this->action_keys[0].action->getParentObject()->setVelocity({ 0,0 });
	}
	
	
	
}
