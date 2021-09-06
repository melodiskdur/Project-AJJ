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

	//Other
	std::vector<Action*> constructActiveActions();
	void triggerActiveActions();
	void breakMovement(int hori_move, int vert_move, float break_value);
	void processUserInput();
	void bindActionToKey(Action* action, sf::Keyboard::Key key);
	void bindActionToKey(Action* action, std::vector<sf::Keyboard::Key> keys);

	//Setters
	void setObject(Object* obj);

	//Getters
	Object* getObject() { return obj; };
	std::vector<Action*> getActiveActions() { return this->active_actions; };

protected:
	Object * obj = nullptr;
	std::vector<ActionKey> action_keys;
	int num_of_active_actions = 0;
	std::vector<Action*> active_actions; 

private:

};

