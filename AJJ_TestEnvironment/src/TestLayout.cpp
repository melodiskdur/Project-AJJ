#include "../includes/TestLayout.h"
#include "../includes/TestButton.h"

Layout* TestLayout::createTestLayout(bool on_fixated_layer)
{
	/*/////////////----ADD LAYOUTS---//////////////*/

	//baselayout (base)
	Layout* test_layout = new Layout(sf::FloatRect(0.f, 0.f, 180, 310), sf::Vector2f(0, 0), sf::Vector2f(0, 0), LAYOUT_PLACEMENT::NONE);

	//base->children
	int num_children = 6;
	int osc = 1;
	for (int i = 0; i < num_children; i++)
	{
		test_layout->addLayout(sf::FloatRect(0, 0, 160, 40), sf::Vector2f(0, 0), sf::Vector2f(0, 10*osc), LAYOUT_PLACEMENT::TOP_CENTERED);
		osc = osc == 1 ? 0 : 1;
	}

	/*/////////////----OPTIONAL----//////////////*/

	//test_layout->setFixedToView(true);
	//test_layout->setBorderSizeForAll({ 10,10 });
	for (Layout* l : test_layout->getLayouts())
	{
		//l->setEnabled(false);
	}

	/*/////////////----FRAMES----//////////////*/

	Frame new_frame;
	new_frame.region_name = "LayoutBlock";
	new_frame.texture_id = TEXTURE_ID::IDLE;
	new_frame.frame_index = 0;
	new_frame.duration = 0;

	test_layout->setCurrentFrameForAll(new_frame);
	test_layout->setTextureNameForAll("LayoutBlock");

	/*/////////////----ACTIONS AND BUTTONS----//////////////*/

	Action* zoom_in = new Action("BtnZoomIn", ACTIONTYPE::ZOOM_IN, -0.01f, 0);
	Action* zoom_out = new Action("BtnZoomOut", ACTIONTYPE::ZOOM_OUT, 0.01f, 0);
	Action* pause = new Action("BtnPause", ACTIONTYPE::PAUSE, 0, 0);
	Action* play = new Action("BtnPlay", ACTIONTYPE::PLAY, 0, 0);
	Action* switch_camera_obj = new Action("BtnSwitchCameraObj", ACTIONTYPE::SWITCH_CAMERA_LOCKED_OBJECT, 0, 0);
	Action* hide_layout = new Action("BtnHideLayout", ACTIONTYPE::HIDE_LAYOUT, 0, 0);

	Button* btn1 = TestButton::createTestButton({ 80,40 }, zoom_in, on_fixated_layer, "ZOOM IN");
	zoom_in->setParentObject(btn1);
	
	Button* btn2 = TestButton::createTestButton({ 80,40 }, zoom_out, on_fixated_layer, "ZOOM OUT");
	zoom_out->setParentObject(btn2);

	Button* btn3 = TestButton::createTestButton({ 80,40 }, pause, on_fixated_layer, "PAUSE");
	pause->setParentObject(btn3);

	Button* btn4 = TestButton::createTestButton({ 80,40 }, play, on_fixated_layer, "PLAY");
	play->setParentObject(btn4);

	Button* btn5 = TestButton::createTestButton({ 80,40 }, switch_camera_obj, on_fixated_layer, "SWITCH");
	switch_camera_obj->setParentObject(btn5);

	Button* btn6 = TestButton::createTestButton({ 80,40 }, hide_layout, on_fixated_layer, "HIDE");
	hide_layout->setParentObject(btn6);

	/*/////////////----ADD OBJECTS----//////////////*/

	test_layout->getLayouts()[0]->addObject(btn1);
	test_layout->getLayouts()[0]->addObject(btn2);
	test_layout->getLayouts()[1]->addObject(btn3);
	test_layout->getLayouts()[1]->addObject(btn4);
	test_layout->getLayouts()[2]->addObject(btn5);
	test_layout->getLayouts()[2]->addObject(btn6);

	/*/////////////----PLACE/ORGANIZE LAYOUTS----//////////////*/

	test_layout->placeLayouts();

	return test_layout;
}