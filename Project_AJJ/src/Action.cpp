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
	//-----------UPDATE ACTION_PARAMETER-----------

	//std::cout << "par_man: " << parameter_manipulation << std::endl;

	//easier to use variables
	sf::Vector2f obj_wrl_pos = this->parent_object->getWorldPosition();
	sf::Vector2f obj_vel = this->parent_object->getVelocity();
	sf::Vector2f obj_max_vel = this->parent_object->getMaxVelocity();

	//check if the action wants to manipulate any parameters
	if (action_parameter == nullptr)
	{
		//this->parent_object->setVelocity({ 0, 0 });

		//if not(the action_parameter equals nullptr), the answer is no.
		//then, do nothing
	}
	//if the actionparameter is the objects x-coordinate
 	else if (*action_parameter == obj_wrl_pos.x)
	{
		//add the parameter_manipulation to the objects velocity in that direction
		this->parent_object->setVelocity({ obj_vel.x + parameter_manipulation, obj_vel.y });

		//update the action parameter with the velocity in that direction
		*action_parameter = *action_parameter + obj_vel.x;
	}
	//else if the actionparameter is the objects y-coordinate
	else if(*action_parameter == obj_wrl_pos.y)
	{
		//add the parameter_manipulation to the objects velocity in that direction
		this->parent_object->setVelocity({ obj_vel.x, obj_vel.y + parameter_manipulation });

		//update the action parameter with the velocity in that direction
		*action_parameter = *action_parameter + obj_vel.y;
	}
	
	//---------------------------------------------------


	//update the objects worldposition
	parent_object->setWorldPosition(parent_object->getWorldPosition());


	//if the action has an animation
	if (this->animation != nullptr)
	{
		//set the texture to the active frame(update sprite)
		parent_object->setFrame(this->animation->getActiveFrame());
		//update the animation, got to the next frame
		this->animation->update();
	}
}