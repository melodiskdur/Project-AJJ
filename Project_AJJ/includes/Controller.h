#pragma once
#include "Object.h"
#include "Action.h"
#include "ExtendedRenderWindow.h"
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

/* Controller
Base class for controlling the object ingame.
*/

struct ActionKey
{
	std::vector<sf::Keyboard::Key> keys;		//the key or key-combination that trigger the action
	sf::Mouse::Button mouse_button;				//the mouse button that trigger the action
	Action* action;								//the action to be triggered
};

class Controller
{
public:
	Controller();
	virtual ~Controller();

	/*Other*/
	//looks at the input from mouse and keyoboard and construct a priority-vector of actions to be triggered
	std::vector<ActionKey> constructActiveActions();
	//trigger the actions in the active_action-vector constructed by constructActiveActions()
	void triggerActiveActions();
	//Experimental!!
	void breakMovement(int hori_move, int vert_move, float break_value);
	//process the user input from keyboard and mouse. Uses constructActiveActions() 
	//in combination with the triggerActiveActions().
	void processUserInput();
	//binds an action to a certain key
	void bindActionToKey(Action* action, sf::Keyboard::Key key);
	//binds an action to a combination of keys
	void bindActionToKey(Action* action, std::vector<sf::Keyboard::Key> keys);
	//binds an action to a mouse button
	void bindActionToMouseButton(Action* action, sf::Mouse::Button button);
	//deacativate the controller
	void deactivateController() { this->controller_state = false; };
	//activate the controller
	void activateController() { this->controller_state = true; };
	//corrects the objects movement depending on its direction. Calling thhis prevents the object
	//from traveling faster on the diagonal.
	void correctMovement();
	//check if the actionkeys are identical
	bool sameActionKey(ActionKey ak1, ActionKey ak2);

	/*Setters*/
	void setWindow(ExtendedRenderWindow* scene);
	void setObject(Object* obj);
	void setNumOfActiveActions(int i) { this->num_of_active_actions = i; };

	/*Getters*/
	bool getControllerState() { return this->controller_state; };
	Object* getObject() { return obj; };
	std::vector<ActionKey> getActiveActionKeys() { return this->active_actionkeys; };
	int getNumOfActiveActions() { return this->num_of_active_actions; };

	static unsigned int instanceCount() { return instance_counter; };
protected:
	Object * obj = nullptr;						//the object connected to the controller
	std::vector<ActionKey> action_keys;			//all action+keys combinations
	int num_of_active_actions = 0;				//the number of active actions
	std::vector<ActionKey> active_actionkeys;	//actionkeys currently activated by user
	bool controller_state = true;				//activated/deactivated i.e. in use or not in use

	sf::Vector2f original_view_size;			//the original view_size
	ExtendedRenderWindow* window = nullptr;		//the main exteneded render window
private:
	static unsigned int instance_counter;
};

