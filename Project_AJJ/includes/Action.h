#pragma once

#include "Object.h"
#include <string>
#include <iostream>

class Object;

enum ACTIONTYPE {MOVE, JUMP};

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
	float* getActionParameter();

	//Setters
	void setActionName(std::string name);
	void setActionType(ACTIONTYPE type);
	void setParentObject(Object* parent);
	void setActionParameter(float* parameter);
	void setParameterManipulation(float manipulation_value);

	//Others
	void triggerAction();

private:
	std::string action_name;
	ACTIONTYPE action_type = ACTIONTYPE::MOVE;
	Object* parent_object = nullptr;
	float* action_parameter;			//Parameter from parent object that the action shall manipulate.
	float parameter_manipulation;	    //The value of which the parameter shall be manipulated by.

	//Animation
};

