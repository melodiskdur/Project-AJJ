#pragma once

#include "Object.h"
#include "Animation.h"
#include <string>
#include <iostream>

class Object;

enum ACTIONTYPE 
{ 
	MOVE_LEFT, 
	MOVE_RIGHT, 
	MOVE_UP, 
	MOVE_DOWN, 
	JUMP, 
	ATTACK, 
	IDLE,
	ZOOM_IN,
	ZOOM_OUT
};

/* Action
*  Provides a way to create actions that manipulate an Object instance's
*  information in some way. For example moving, jumping, etc.
*/
class Action
{
public:
	Action();
	~Action();

	//Getters
	std::string getActionName();
	ACTIONTYPE getActionType();
	Object* getParentObject();
	float getParameterManipulation() { return this->parameter_manipulation; };
	Animation* getAnimation();

	//Setters
	void setActionName(std::string name);
	void setActionType(ACTIONTYPE type);
	void setParentObject(Object* parent);
	void setParameterManipulation(float manipulation_value);
	void setAnimation(Animation* animation);

	//Others
	void triggerAction();

private:
	std::string action_name;
	ACTIONTYPE action_type = ACTIONTYPE::IDLE;
	Object* parent_object = nullptr;
	float parameter_manipulation = 0;	    //The value of which the parameter shall be manipulated by.

	//Animation
	Animation* animation = nullptr;		//An animation, sequence of sprites
};

