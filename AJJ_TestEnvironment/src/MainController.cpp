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

	ACTIONTYPE type = this->active_actions[index]->getActionType();		//the current actions type
	Camera* camera = this->window->getActiveScene()->getCamera();		//the window camera
	bool window_active = this->window->getWindowState();				//if the window is active or not


	//in-game main-controller actions 
	if (this->menu->getActiveMenuAlternative() == MENU_ALTERNATIVES::PLAY_GAME)
	{
		//PAUSE
		if (type == ACTIONTYPE::PAUSE && window_active)
		{
			for (auto& contr : this->controllers)
			{
				contr->Controller::deactivateController();
			}
			this->window->deactivateWindow();
		}
		//PLAY
		if (type == ACTIONTYPE::PLAY && !window_active)
		{
			for (auto& contr : this->controllers)
			{
				contr->Controller::activateController();
			}
			this->window->activateWindow();
		}
		//ZOOM IN/OUT
		if ((type == ACTIONTYPE::ZOOM_IN || type == ACTIONTYPE::ZOOM_OUT) && window_active)
		{
			//get the current zoom_factor
			float cur_zoom = camera->getCameraZoom();
			//set the view-size to the original one. Needed
			camera->getCameraView()->setSize(this->original_view_size);
			//zoom the view by the current zoom_factor + the actions specific parameter manipulation value
			camera->setCameraZoom(cur_zoom + this->active_actions[index]->getParameterManipulation());
		}
		//SWITCH CAMERA OBJECT
		if (type == ACTIONTYPE::SWITCH_CAMERA_LOCKED_OBJECT && window_active)
		{
			//get the id of the object that the camera is currently locked on
			int locked_obj_id = camera->getTargetObject()->getId();

			//loop all controllers
			for (int i = 0; i < this->num_of_controllers; i++)
			{
				//if one of the controllers has the obj the camera is currently locked on
				if (this->controllers[i]->getObject()->getId() == locked_obj_id)
				{
					//check if its the last element in the controllers vector
					if (i + 1 == this->num_of_controllers)
					{
						//if so, go to the beginning
						i = 0;
					}
					else
					{
						//else, go to the next controller in the vector
						i++;
					}

					//lock the camera on the new object
					camera->lockOnObject(this->controllers[i]->getObject());

					//and end the loop
					break;
				}
			}
		}
	}
	
	//menu main-controller actions
	if (this->menu->getActiveMenuAlternative() == MENU_ALTERNATIVES::MAIN_MENU && window_active)
	{
		//MOVE UP/DOWN IN THE MENU
		if (type == ACTIONTYPE::MENU_MOVE_DOWN || type == ACTIONTYPE::MENU_MOVE_UP)
		{
			//get focused/highlighted menu alternative
			MENU_ALTERNATIVES foc_a = this->menu->getFocusedMenuAlternative();
			//std::cout << foc_a << std::endl;

			//get parameter manipulation value for action (1 = down, -1 = up)
			int par_m = this->active_actions[index]->getParameterManipulation();

			//check if bounds with the maximum number of alternatives
			//i.e. if we can move up/down
			if (foc_a + par_m <= MAXIMUM_NUM_ALTERNATIVES - 1 && foc_a + par_m >= 0)
			{
				//if so, apply parameter manipulation value and change focused alternative
				this->menu->setFocusedMenuAlternative(static_cast<MENU_ALTERNATIVES>(foc_a + par_m));
			}
		}
		//CHOOSE MENU ALTERNATIVE
		if (type == ACTIONTYPE::MENU_CHOOSE_ALTERNATIVE)
		{
			//chamge active menu alternative to the one in focus
			this->menu->setActiveMenuAlternative(this->menu->getFocusedMenuAlternative());
			//std::cout << this->menu->getActiveMenuAlternative() << std::endl;
		}
	}
	


	//TO BE CONTINUED.....
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
	//add to active_actions vector
	Controller::constructActiveActions();

	//if there are no active_actions
	if (active_actions.empty())
	{
		active_actions.push_back(action_keys[0].action);				//add the idle/not-active action
		this->num_of_active_actions++;
	}

	//at last, trigger all active_actions
	MainController::triggerActiveActions();

}