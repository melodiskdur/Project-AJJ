#pragma once
#include "Object.h"
//#include "Action.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

/* Controller
Base class for controlling the character ingame.
*/
class Controller
{
public:
	Controller();
	virtual ~Controller();

	//update an objects w.pos. with setWorldPosition()
	void updateObjectWorldPosition(Object& obj);
	void useAction();

protected:
	

private:

};

