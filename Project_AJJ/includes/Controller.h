#pragma once
#include "Object.h"
#include "Action.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

/* Controller
Base class for controlling the object ingame.
*/

struct ActionKey
{
	sf::Keyboard::Key key;
	Action* action;
};

class Controller
{
public:
	Controller();
	virtual ~Controller();

	void getUserInput();
	void bindActionToKey(Action* action, sf::Keyboard::Key key);

	//Setters
	void setObject(Object* obj);

	//Getters
	Object* getObject() { return obj; };

protected:
	Object * obj = nullptr;
	std::vector<ActionKey> action_keys;

private:

};

