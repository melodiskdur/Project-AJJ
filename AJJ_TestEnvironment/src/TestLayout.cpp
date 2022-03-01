#include "../includes/TestLayout.h"



Layout* TestLayout::createTestLayout()
{
	//baselayout (base)
	Layout* test_layout = new Layout(sf::FloatRect(300, -400, 500, 400), sf::Vector2f(0, 0), sf::Vector2f(0, 0), LAYOUT_PLACEMENT::LP_NONE);

	//base->children
	test_layout->addLayout(sf::FloatRect(0, 0, 460, 60), sf::Vector2f(0, 0), sf::Vector2f(20, 20), LAYOUT_PLACEMENT::LP_TOP_CENTERED);

	//base->children[0]->children
	test_layout->getLayouts()[0]->addLayout(sf::FloatRect(0, 0, 440, 40), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED);

	//this layout will be fixed and adjustment to be in the users view
	test_layout->setFixedToView(false);

	Frame new_frame;
	new_frame.region_name = "LayoutBlock";
	new_frame.texture_id = TEXTURE_ID::IDLE;
	new_frame.frame_index = 0;
	new_frame.duration = 0;

	test_layout->setFrameForAll(new_frame);
	test_layout->setTextureNameForAll("LayoutBlock");

	//test_layout->setBorderSizeForAll({ 10,10 });

	//add a test-button
	Button* btn = new Button({ 300,-100 }, { 80,40 });
	Button* btn2 = new Button({ 300,-70 }, { 80,40 });

	Frame def_btn;
	def_btn.texture_id = TEXTURE_ID::IDLE;
	def_btn.region_name = "MenuButtons";
	def_btn.frame_index = 0;

	Frame hov_btn;
	hov_btn.texture_id = TEXTURE_ID::HOVER;
	hov_btn.region_name = "MenuButtons";
	hov_btn.frame_index = 1;

	Frame prs_btn;
	prs_btn.texture_id = TEXTURE_ID::PRESSED;
	prs_btn.region_name = "MenuButtons";
	prs_btn.frame_index = 2;

	btn->setCurrentFrame(def_btn);
	btn->setDefaultFrame(def_btn);
	btn->setHoverFrame(hov_btn);
	btn->setPressedFrame(prs_btn);

	btn->setTextureName("MenuButtons");

	btn2->setCurrentFrame(def_btn);
	btn2->setDefaultFrame(def_btn);
	btn2->setHoverFrame(hov_btn);
	btn2->setPressedFrame(prs_btn);

	btn2->setTextureName("MenuButtons");

	Action* btn_prs = new Action;
	btn_prs->setParameterManipulation(-0.01f);
	btn_prs->setActionName("ButtonPause");
	btn_prs->setParentObject(btn);
	btn_prs->setActionType(ACTIONTYPE::AT_ZOOM_IN);

	Action* btn_prs2 = new Action;
	btn_prs2->setParameterManipulation(0.01f);
	btn_prs2->setActionName("ButtonPlay");
	btn_prs2->setParentObject(btn);
	btn_prs2->setActionType(ACTIONTYPE::AT_ZOOM_OUT);


	btn->addAction(btn_prs);
	btn2->addAction(btn_prs2);

	btn->loadFont("../Project_AJJ/assets/ayar_takhu.ttf");
	btn2->loadFont("../Project_AJJ/assets/ayar_takhu.ttf");
	btn->setText(btn->getSize().x / 8, sf::Text::Style::Bold, sf::Color::White, "ZOOM IN");
	btn2->setText(btn2->getSize().x / 8, sf::Text::Style::Bold, sf::Color::White, "ZOOM OUT");
	
	test_layout->getLayouts()[0]->getLayouts()[0]->addObject(btn);
	test_layout->getLayouts()[0]->getLayouts()[0]->addObject(btn2);


	//place them all
	test_layout->placeLayouts();

	return test_layout;
}