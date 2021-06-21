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
	new_actionkey.key = key;
	this->action_keys.push_back(new_actionkey);
}

void Controller::setObject(Object* obj)
{
	this->obj = obj;
}

void Controller::getUserInput()
{
	for (int i = 0; i < action_keys.size(); i++)
	{
		if (sf::Keyboard::isKeyPressed(action_keys[i].key))
		{
			action_keys[i].action->triggerAction();
		}
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
