#pragma once
#include "Controller.h"
#include "Action.h"
#include "ExtendedRenderWindow.h"

class MainController : 
	public Controller
{
public:
	MainController();
	~MainController();

	//Setters


	//Getters

	//Other
	void activateController(Controller* controller);
	void deactivateController(Controller* controller);
	void addController(Controller* new_controller);
	void triggerAction(int index);
	void triggerActiveActions();
	void processUserInput();
	void setWindow(ExtendedRenderWindow* scene);

private:
	sf::Vector2f original_view_size;
	ExtendedRenderWindow* window = nullptr;
	std::vector<Controller*> controllers;
	int num_of_controllers = 0;

};

