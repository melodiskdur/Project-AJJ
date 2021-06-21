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

//Other
void Action::triggerAction()
{
	*action_parameter = *action_parameter + parameter_manipulation;
	//parent_object->doStuff
}