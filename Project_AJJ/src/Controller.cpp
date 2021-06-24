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

void Controller::getUserInput()
{
	bool action_state;												//Assumes action state.
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
			action_keys[i].action->triggerAction();					 //If the action state remained true, action is triggered.
	}
	/*
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		obj->onActionRequest("Left");
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		obj->onActionRequest("Right");
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		obj->onActionRequest("Up");
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		obj->onActionRequest("Down");*/
}
