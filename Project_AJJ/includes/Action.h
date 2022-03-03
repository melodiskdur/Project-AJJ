#pragma once

#include "Object.h"
#include "Animation.h"
#include <string>
#include <iostream>
#include <math.h>

class Object;

enum class ACTIONTYPE 
{ 
	MOVE_LEFT, 
	MOVE_RIGHT, 
	MOVE_UP, 
	MOVE_DOWN, 
	JUMP, 
	ATTACK, 
	IDLE,
	ZOOM_IN,
	ZOOM_OUT,
	PAUSE,
	PLAY,
	STEP_FORWARD,
	STEP_BACK,
	SWITCH_CAMERA_LOCKED_OBJECT,
	MENU_MOVE_UP,
	MENU_MOVE_DOWN,
	MENU_CHOOSE_ALTERNATIVE,
	EXIT_TO_MENU,
	HIDE_LAYOUT
};

/* Action
*  Provides a way to create actions that manipulate an Object instance's
*  information in some way. For example moving, jumping, etc.
*/
class Action
{
public:
	Action();
	Action(sf::String action_name, ACTIONTYPE action_type, float parameter_manipulation, float action_duration);
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

	static unsigned int instanceCount() { return instance_counter; };
private:
	std::string action_name;
	ACTIONTYPE action_type = ACTIONTYPE::IDLE;
	Object* parent_object = nullptr;
	float parameter_manipulation = 0;	    //The value of which the parameter shall be manipulated by.
	float action_duration = 0;				//Amount of time the action is run.

	//Animation
	Animation* animation = nullptr;		//An animation, sequence of sprites

	static unsigned int instance_counter;
};

