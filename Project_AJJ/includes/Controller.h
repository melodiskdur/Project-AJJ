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
	void deactivateController() { this->controller_state = false; };
	void activateController() { this->controller_state = true; };

	//Setters
	void setObject(Object* obj);
	void setNumOfActiveActions(int i) { this->num_of_active_actions = i; };

	//Getters
	bool getControllerState() { return this->controller_state; };
	Object* getObject() { return obj; };
	std::vector<Action*> getActiveActions() { return this->active_actions; };
	int getNumOfActiveActions() { return this->num_of_active_actions; };

protected:
	Object * obj = nullptr;						//the object connected to the controller
	std::vector<ActionKey> action_keys;			//all action+keys combinations
	int num_of_active_actions = 0;				
	std::vector<Action*> active_actions;		//actions currently activated by user
	bool controller_state = true;				//activated/deactivated

private:

};

