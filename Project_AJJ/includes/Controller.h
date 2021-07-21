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
	std::vector<sf::Keyboard::Key> keys;
	Action* action;
};

class Controller
{
public:
	Controller();
	virtual ~Controller();

	void processUserInput();
	void bindActionToKey(Action* action, sf::Keyboard::Key key);
	void bindActionToKey(Action* action, std::vector<sf::Keyboard::Key> keys);

	//Setters
	void setObject(Object* obj);

	//Getters
	Object* getObject() { return obj; };

protected:
	Object * obj = nullptr;
	std::vector<ActionKey> action_keys;

private:

};

