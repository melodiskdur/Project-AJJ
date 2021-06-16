#pragma once
#include "Object.h"
//#include "Action.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

/* Controller
Base class for controlling the object ingame.
*/
class Controller
{
public:
	Controller(Object& obj);
	virtual ~Controller();

	//update an objects w.pos. with setWorldPosition()
	void updateObjectWorldPosition();

	//use certain action(jump,run,croutch etc.)
	void useAction();

	//returns the current action being used
	int getAction();	

protected:
	int activeAction;
	Object obj;

private:

};

