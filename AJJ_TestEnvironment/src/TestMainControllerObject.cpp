#include "../includes/TestMainControllerObject.h"

TestMainControllerObject::TestMainControllerObject()
	: Object()
{
	/*CREATE A NEW ACTION
	* 1.Action * new_action = new Action;
	* 2. new_action->setActionName("name");
	* 3. new_action->setParentObject(this);
	* 4. new_action->setActionType(ACTIONTYPE::SOME_TYPE);
	* 5. object_actions.pushback(new_action);
	* 6. Done!
	*/

	Action* idle = new Action;
	Action* zoom_in = new Action;
	Action* zoom_out = new Action;
	Action* switch_camera_locked_object = new Action;
	Action* pause = new Action;
	Action* play = new Action;
	Action* menu_move_up = new Action;
	Action* menu_move_down = new Action;
	Action* menu_choose_alternative = new Action;
	Action* exit_to_menu = new Action;

	idle->setActionName("Idle");
	idle->setParentObject(this);
	idle->setActionType(ACTIONTYPE::IDLE);

	zoom_in->setActionName("ZoomIn");
	zoom_in->setParameterManipulation(-0.01);
	zoom_in->setParentObject(this);
	zoom_in->setActionType(ACTIONTYPE::ZOOM_IN);

	zoom_out->setActionName("ZoomOut");
	zoom_out->setParameterManipulation(0.01);
	zoom_out->setParentObject(this);
	zoom_out->setActionType(ACTIONTYPE::ZOOM_OUT);

	switch_camera_locked_object->setActionName("SwitchCameraLockedObject");
	switch_camera_locked_object->setParentObject(this);
	switch_camera_locked_object->setActionType(ACTIONTYPE::SWITCH_CAMERA_LOCKED_OBJECT);

	pause->setActionName("Pause");
	pause->setParentObject(this);
	pause->setActionType(ACTIONTYPE::PAUSE);

	play->setActionName("Play");
	play->setParentObject(this);
	play->setActionType(ACTIONTYPE::PLAY);

	menu_move_down->setActionName("MenuMoveDown");
	menu_move_down->setParameterManipulation(1);	//increment on y-axis i.e. down
	menu_move_down->setParentObject(this);
	menu_move_down->setActionType(ACTIONTYPE::MENU_MOVE_DOWN);

	menu_move_up->setActionName("MenuMoveUp");
	menu_move_up->setParameterManipulation(-1);		//reduction on y-axis i.e. up
	menu_move_up->setParentObject(this);
	menu_move_up->setActionType(ACTIONTYPE::MENU_MOVE_UP);

	menu_choose_alternative->setActionName("MenuChooseAlternative");
	menu_choose_alternative->setParentObject(this);
	menu_choose_alternative->setActionType(ACTIONTYPE::MENU_CHOOSE_ALTERNATIVE);

	exit_to_menu->setActionName("ExitToMenu");
	exit_to_menu->setParentObject(this);
	exit_to_menu->setActionType(ACTIONTYPE::EXIT_TO_MENU);

	object_actions.push_back(idle);
	object_actions.push_back(zoom_in);
	object_actions.push_back(zoom_out);
	object_actions.push_back(switch_camera_locked_object);
	object_actions.push_back(pause);
	object_actions.push_back(play);
	object_actions.push_back(menu_move_down);
	object_actions.push_back(menu_move_up);
	object_actions.push_back(menu_choose_alternative);
	object_actions.push_back(exit_to_menu);

}

TestMainControllerObject::~TestMainControllerObject()
{

}