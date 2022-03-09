#include "MainController.h"

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

void MainController::addController(Controller* new_controller)
{
	this->controllers.push_back(new_controller);
	this->num_of_controllers++;
}

void MainController::triggerAction(int index)
{
	ACTIONTYPE type = this->active_actionnodes[index].action->getActionType();		//the current actions type
	ActionNode action_node = this->active_actionnodes[index];							//
	Camera* camera = this->window->getActiveScene()->getCamera();					//the window camera
	bool window_active = this->window->getWindowState();							//if the window is active or not
	Scene* active_scene = this->window->getActiveScene();							//the windows active scene

	//in-game main-controller actions 
	if (active_scene->getSceneDenotation() == SCENE_DENOTATION::TEST_GAME)
	{
		//PAUSE
		if ((type == ACTIONTYPE::PAUSE && window_active) || (type == ACTIONTYPE::PLAY && window_active))
		{
			for (auto& contr : this->controllers)
			{
				contr->Controller::deactivateController();
			}
			this->window->deactivateWindow();
		}
		//PLAY
		if ((type == ACTIONTYPE::PLAY && !window_active) || (type == ACTIONTYPE::PAUSE && !window_active))
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
			camera->setCameraZoom(cur_zoom + this->active_actionnodes[index].action->getParameterManipulation());
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
		//HIDE LAYOUT
		if (type == ACTIONTYPE::HIDE_LAYOUT && window_active)
		{
			//HARDCODED LAYER WITH NUM -3!!!!!!!!!!!!!
			for (auto& l : this->window->getActiveScene()->getLayer(-3)->getLayouts())
			{
				if (l->getEnabled())
				{
					l->setEnabledForAll(false);
				}
				else
				{
					l->setEnabledForAll(true);
				}

			}
		}
		//EXIT TO MAIN MENU
		if (type == ACTIONTYPE::EXIT_TO_MENU)
		{
			
		}
	}

	//menu main-controller actions
	if (active_scene->getSceneDenotation() == SCENE_DENOTATION::MAIN_MENU && window_active)
	{
		//MOVE UP/DOWN IN THE MENU
		if (type == ACTIONTYPE::MENU_MOVE_DOWN || type == ACTIONTYPE::MENU_MOVE_UP)
		{

		}
		//CHOOSE MENU ALTERNATIVE
		if (type == ACTIONTYPE::MENU_CHOOSE_ALTERNATIVE)
		{

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
	for (int i = this->num_active_actionnodes - 1; i >= 0; i--)
	{
		//std::cout << this->active_actions[i]->getActionName() << std::endl;
		MainController::triggerAction(i);
	}
}

void MainController::processUserInput()
{
	//DEBUGGING
	// Search for "correct" SceneLayer-view.
	// NOTE: HARDCODED. TEMPORARY CODE.
	std::map<int, sf::View> lv_pairs = this->window->getActiveScene()->getLayerManipulatedView();
	sf::View v;
	for (const std::pair<int, sf::View>& p : lv_pairs)
	{ if (p.first == -3) { v = p.second; break; } }
	this->window->setView(v);
	 
	//update cursor specific parameters
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !getMouseButtonState(sf::Mouse::Button::Left))
	{
		if (this->window != nullptr)
			setLastCursorPress(sf::Mouse::getPosition(*this->window));
		//std::cout << "\nlast_cursor_press_pos: " << last_cursor_press_pos.x << ", " << last_cursor_press_pos.y << std::endl;	

	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && getMouseButtonState(sf::Mouse::Button::Left))
	{
		if (this->window != nullptr)
			setLastCursorRelease(sf::Mouse::getPosition(*this->window));
		//std::cout << "last_cursor_release_pos: " << last_cursor_release_pos.x << ", " << last_cursor_release_pos.y << std::endl;
	}
	for (auto& contr : this->controllers)
	{
		contr->setLastCursorPress(this->last_cursor_press_pos);
		contr->setLastCursorRelease(this->last_cursor_release_pos);
	}
	//END DEBUGGING
	

	//add to active_actions vector
	constructActiveActions();


	//DEBUGGING
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !getMouseButtonState(sf::Mouse::Button::Left))
	{
		this->setMouseButtonState(sf::Mouse::Button::Left, true);
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && getMouseButtonState(sf::Mouse::Button::Left))
	{
		this->setMouseButtonState(sf::Mouse::Button::Left, false);
	}
	for (auto& contr : this->controllers)
	{
		contr->setMouseButtonState(sf::Mouse::Button::Left, this->left_mouse_button_state);
		contr->setMouseButtonState(sf::Mouse::Button::Right, this->right_mouse_button_state);
	}
	//END DEBUGGING


	//if there are no active_actions
	if (active_actionnodes.empty())
	{
		active_actionnodes.push_back(actionnodes[0]);				//add the idle/not-active action
		this->num_active_actionnodes++;
	}
	//at last, trigger all active_actions
	MainController::triggerActiveActions();

	for (Controller* contr : this->controllers)
	{
		contr->processUserInput();
	}

}