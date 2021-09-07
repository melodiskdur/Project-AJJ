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

	object_actions.push_back(idle);
	object_actions.push_back(zoom_in);
	object_actions.push_back(zoom_out);
	object_actions.push_back(switch_camera_locked_object);
	object_actions.push_back(pause);
	object_actions.push_back(play);
}

TestMainControllerObject::~TestMainControllerObject()
{

}