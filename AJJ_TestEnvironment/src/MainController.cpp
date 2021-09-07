#include "../includes/MainController.h"

MainController::MainController()
	: Controller()
{

}

MainController::~MainController()
{

}

void MainController::activateController(Controller* controller)
{
	controller->Controller::activateController();
}

void MainController::deactivateController(Controller* controller)
{
	controller->Controller::deactivateController();
}

void MainController::addController(Controller * new_controller)
{ 
	this->controllers.push_back(new_controller); 
	this->num_of_controllers++;
}

void MainController::setWindow(ExtendedRenderWindow* window)
{
	this->window = window;
	this->original_view_size = this->window->getActiveScene()->getCamera()->getCameraView()->getSize();
}

void MainController::triggerAction(int index)
{
	ACTIONTYPE type = this->active_actions[index]->getActionType();

	Camera* camera = this->window->getActiveScene()->getCamera();

	if (type == ACTIONTYPE::PAUSE && this->window->getWindowState())
	{
		for (auto& contr : this->controllers)
		{
			contr->Controller::deactivateController();
		}
		this->window->deactivateWindow();
	}
	if (type == ACTIONTYPE::PLAY && !this->window->getWindowState())
	{
		for (auto& contr : this->controllers)
		{
			contr->Controller::activateController();
		}
		this->window->activateWindow();
	}
	if (type == ACTIONTYPE::ZOOM_IN || type == ACTIONTYPE::ZOOM_OUT)
	{
		//get the current zoom_factor
		float cur_zoom = camera->getCameraZoom();
		//set the view-size to the original one. Needed
		camera->getCameraView()->setSize(this->original_view_size);
		//zoom the view by the current zoom_factor + the actions specific parameter manipulation value
		camera->setCameraZoom(cur_zoom + this->active_actions[index]->getParameterManipulation());
	}
	if (type == ACTIONTYPE::SWITCH_CAMERA_LOCKED_OBJECT)
	{
		//get the id of the object that the camera is currently locked on
		int locked_obj_id = camera->getTargetObject()->getId();

		//HARDCODED FOR PLAYER_1 AND 2. ID: 420 AND 1337
		//change id to other player
		if (locked_obj_id == 420)
			locked_obj_id = 1337;
		else if (locked_obj_id == 1337)
			locked_obj_id = 420;
		else
		{
			//ERROR
			locked_obj_id = 0;
		}

		//lock the camera to the next object
		camera->lockOnObject(this->window->getActiveScene()->getObjectWithId(locked_obj_id));
	}
	if (type == ACTIONTYPE::STEP_FORWARD)
	{

	}
	if (type == ACTIONTYPE::STEP_BACK)
	{

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