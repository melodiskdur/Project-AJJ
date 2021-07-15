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

float* Action::getActionParameter()
{
	return action_parameter;
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

void Action::setActionParameter(float* parameter)
{
	action_parameter = parameter;
}

void Action::setParameterManipulation(float manipulation_value)
{
	parameter_manipulation = manipulation_value;
}

void Action::setAnimation(Animation* animation)
{
	this->animation = animation;
}

//Use the action, apply the parameter manipulation
//update pos and the animation
void Action::triggerAction()
{
	//update the action parameter
	*action_parameter = *action_parameter + parameter_manipulation;

	//update the objects worldposition
	parent_object->setWorldPosition(parent_object->getWorldPosition());

	//if the action has an animation
	if (this->animation != nullptr)
	{
		//set the texture to the active frame
		parent_object->setFrame(this->animation->getActiveFrame());
		//update the animation
		this->animation->update();
	}

	//parent_object->doStuff
}