#pragma once
#include "Controller.h"
#include "Action.h"

class MainController :
	public Controller
{
public:
	MainController();
	~MainController();

	//Setters
	
	//Getters
	ExtendedRenderWindow* getWindow() { return this->window; }
	std::vector<Controller*> getControllers() { return this->controllers; }
	int getNumControllers() { return this->num_of_controllers; }

	//Other
	void activateController(Controller* controller);
	void deactivateController(Controller* controller);

	void addController(Controller* new_controller);

	void triggerAction(int index);
	void triggerActiveActions();

	void processUserInput();


private:
	std::vector<Controller*> controllers;		//all of the object controllers in the game
	int num_of_controllers = 0;					//the number of controllers

};


