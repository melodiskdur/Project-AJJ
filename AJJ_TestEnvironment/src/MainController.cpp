#include "../includes/MainController.h"

MainController::MainController()
	: Controller()
{

}

MainController::~MainController()
{

}

void MainController::triggerAction(int index)
{
	if (this->active_actions[index]->getActionType() == ACTIONTYPE::ZOOM_IN || 
		this->active_actions[index]->getActionType() == ACTIONTYPE::ZOOM_OUT)
	{
		float cur_zoom = this->scene->getCamera()->getCameraZoom();
		this->scene->getCamera()->getCameraView()->setSize(800, 600); ///HHEEEEEEHUHWUHFUWH
		this->scene->getCamera()->setCameraZoom(cur_zoom + this->active_actions[index]->getParameterManipulation());
	}
}

void MainController::triggerActiveActions()
{
	for (int i = this->num_of_active_actions - 1; i >= 0; i--)
	{
		//std::cout << this->active_actions[i]->getActionName() << std::endl;
		MainController::triggerAction(i);
	}
}

void MainController::processUserInput()
{
	Controller::constructActiveActions();

	//if there are no active_actions
	if (active_actions.empty())
	{
		active_actions.push_back(action_keys[0].action);				//add the idle/not-active action
		this->num_of_active_actions++;
	}

	MainController::triggerActiveActions();

}