#pragma once
#include "Controller.h"
#include "Action.h"
#include "Scene.h"

class MainController : 
	public Controller
{
public:
	MainController();
	~MainController();

	void triggerAction(int index);
	void triggerActiveActions();
	void processUserInput();
	void setScene(Scene* scene) { this->scene = scene; };

private:
	Scene* scene = nullptr;

};

