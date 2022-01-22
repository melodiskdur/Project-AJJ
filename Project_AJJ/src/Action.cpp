#include "Action.h"

Action::Action()
{

}

Action::~Action()
{

}

//Getters
std::string Action::getActionName()
{
	return action_name;
}

ACTIONTYPE Action::getActionType()
{
	return action_type;
}

Object* Action::getParentObject()
{
	return parent_object;
}

Animation* Action::getAnimation()
{
	return this->animation;
}

//Setters
void Action::setActionName(std::string name)
{
	action_name = name;
}

void Action::setActionType(ACTIONTYPE type)
{
	action_type = type;
}

void Action::setParentObject(Object* parent)
{
	parent_object = parent;
}

void Action::setParameterManipulation(float manipulation_value)
{
	parameter_manipulation = manipulation_value;
}

void Action::setAnimation(Animation* animation)
{
	this->animation = animation;
}

void Action::triggerAction()
{
	/*triggerAction
	* This function triggers the specific action
	*/

	sf::Vector2f set_vel = this->parent_object->getVelocity();
	
	//horizontal movement
	if (this->action_type == ACTIONTYPE::MOVE_LEFT || this->action_type == ACTIONTYPE::MOVE_RIGHT)
	{
		set_vel.x += this->parameter_manipulation;
	}
	//vertical movement
	else if (this->action_type == ACTIONTYPE::MOVE_UP || this->action_type == ACTIONTYPE::MOVE_DOWN)
	{
		set_vel.y += this->parameter_manipulation;
	}

	else if (this->action_type == ACTIONTYPE::ATTACK)
	{
		//DO SOMETHING
	}
	else if (this->action_type == ACTIONTYPE::IDLE)
	{
		//DO SOMETHING
		//If an object has any actions other than Idle, Idle should also be added
		//always index 0 in object_actions vector
	}
	else
	{
		//OBJECT HAS NO IDLE STATE, ADD IT
	}

	//set the velocity to the updated one
	this->parent_object->setVelocity(set_vel);
	
	//if the action has an animation, update it
	if (this->animation != nullptr)
	{
		//set the texture to the active frame(update sprite)
		parent_object->setFrame(this->animation->getActiveFrame());
		//update the animation(got to the next frame)
		this->animation->updateAnimation();
	}
}