#pragma once
#include "Object.h"
#include "Action.h"
#include "ExtendedRenderWindow.h"
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

/* Controller
Base class for controlling the object ingame.
*/
struct ActionNode
{
	std::vector<sf::Keyboard::Key> keys;		//the key or key-combination that trigger the action
	sf::Mouse::Button mouse_button;				//the mouse button that trigger the action
	Action* action = nullptr;					//the action to be triggered
	Object* obj = nullptr;						//the object related to the action
	int id;										//action_key identifier
};

class Controller
{
public:
	Controller();
	virtual ~Controller();

	/*Other*/
	//looks at the input from mouse and keyoboard and construct a priority-vector of actions to be triggered
	std::vector<ActionNode> constructActiveActions();
	//trigger the actions in the active_action-vector constructed by constructActiveActions()
	void triggerActiveActions();
	//process the user input from keyboard and mouse. Uses constructActiveActions() 
	//in combination with the triggerActiveActions().
	void processUserInput();
	//binds an action to a certain key
	void bindActionToKey(Action* action, sf::Keyboard::Key key);
	//binds an action to a combination of keys
	void bindActionToKey(Action* action, std::vector<sf::Keyboard::Key> keys);
	//binds an action to a mouse button
	void bindActionToMouseButton(Action* action, sf::Mouse::Button button);
	//binds an action to a mouse button
	void bindActionToMouseButton(Action* action, Object* obj, sf::Mouse::Button button);
	//deacativate the controller
	void deactivateController() { this->controller_state = false; };
	//activate the controller
	void activateController() { this->controller_state = true; };
	//corrects the objects movement depending on its direction. Calling thhis prevents the object
	//from traveling faster on the diagonal.
	void correctMovement();
	//check if the actionnodes are identical
	bool sameActionnode(ActionNode actionnode_1, ActionNode actionnode_2);

	//returns true or false if the cursor is over the rect
	bool cursorHover(sf::FloatRect rect);
	//returns a vector of rects that the cursor is hovering from a set of rects
	std::vector<sf::FloatRect> cursorHoverRects(std::vector<sf::FloatRect> rects);

	/*Setters*/
	void setWindow(ExtendedRenderWindow* scene);
	void setObject(Object* obj);

	/*Getters*/
	bool getControllerState() { return this->controller_state; };
	Object* getObject() { return obj; };
	std::vector<ActionNode> getActiveActionnodes() { return this->active_actionnodes; };
	int getNumActiveActionnodes() { return this->num_active_actionnodes; };
	int getNumActionnodes() { return this->num_actionnodes; };

	static unsigned int instanceCount() { return instance_counter; };
protected:
	Object * obj = nullptr;						//the object connected to the controller

	std::vector<ActionNode> actionnodes;			//all action+keys combinations
	int num_actionnodes = 0;						//the number of actionnodes

	std::vector<ActionNode> active_actionnodes;	//actionnodes currently activated by user
	int num_active_actionnodes = 0;				//the number of active actions

	bool controller_state = true;				//activated/deactivated i.e. in use or not in use

	sf::Vector2f original_view_size;			//the original view_size
	ExtendedRenderWindow* window = nullptr;		//the main exteneded render window
private:
	static unsigned int instance_counter;
};

