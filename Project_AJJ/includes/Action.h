#pragma once

#include "Object.h"
#include "Animation.h"
#include <string>
#include <iostream>
#include <math.h>

class Object;

enum ACTIONTYPE 
{ 
	AT_MOVE_LEFT, 
	AT_MOVE_RIGHT, 
	AT_MOVE_UP, 
	AT_MOVE_DOWN, 
	AT_JUMP, 
	AT_ATTACK, 
	AT_IDLE,
	AT_ZOOM_IN,
	AT_ZOOM_OUT,
	AT_PAUSE,
	AT_PLAY,
	AT_STEP_FORWARD,
	AT_STEP_BACK,
	AT_SWITCH_CAMERA_LOCKED_OBJECT,
	AT_MENU_MOVE_UP,
	AT_MENU_MOVE_DOWN,
	AT_MENU_CHOOSE_ALTERNATIVE,
	AT_EXIT_TO_MENU,
	AT_BUTTON_PRESSED
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

	static unsigned int instanceCount() { return instance_counter; };
private:
	std::string action_name;
	ACTIONTYPE action_type = ACTIONTYPE::AT_IDLE;
	Object* parent_object = nullptr;
	float parameter_manipulation = 0;	    //The value of which the parameter shall be manipulated by.
	float action_duration = 0;				//Amount of time the action is run. -1 is default, meaing 

	//Animation
	Animation* animation = nullptr;		//An animation, sequence of sprites

	static unsigned int instance_counter;
};

