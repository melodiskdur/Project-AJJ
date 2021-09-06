#include "../includes/TestMainControllerObject.h"

TestMainControllerObject::TestMainControllerObject()
	: Object()
{
	Action* idle = new Action;
	Action* zoom_in = new Action;
	Action* zoom_out = new Action;

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

	object_actions.push_back(idle);
	object_actions.push_back(zoom_in);
	object_actions.push_back(zoom_out);
}

TestMainControllerObject::~TestMainControllerObject()
{

}