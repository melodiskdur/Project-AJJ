#include "../includes/TestLayout.h"



Layout* TestLayout::createTestLayout()
{
	//baselayout (base)
	Layout* test_layout = new Layout(sf::FloatRect(300, -400, 500, 400), sf::Vector2f(0, 0), sf::Vector2f(0, 0), LAYOUT_PLACEMENT::LP_NONE);

	//base->children
	test_layout->addLayout(sf::FloatRect(0, 0, 460, 60), sf::Vector2f(0, 0), sf::Vector2f(20, 20), LAYOUT_PLACEMENT::LP_TOP_CENTERED);
	test_layout->addLayout(sf::FloatRect(0, 0, 210, 250), sf::Vector2f(0, 0), sf::Vector2f(20, 15), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->addLayout(sf::FloatRect(0, 0, 210, 40), sf::Vector2f(0, 0), sf::Vector2f(20, 15), LAYOUT_PLACEMENT::LP_TOP_RIGHT);
	test_layout->addLayout(sf::FloatRect(0, 0, 210, 40), sf::Vector2f(0, 0), sf::Vector2f(20, 15), LAYOUT_PLACEMENT::LP_TOP_RIGHT);
	test_layout->addLayout(sf::FloatRect(0, 0, 210, 40), sf::Vector2f(0, 0), sf::Vector2f(20, 15), LAYOUT_PLACEMENT::LP_TOP_RIGHT);
	test_layout->addLayout(sf::FloatRect(0, 0, 210, 40), sf::Vector2f(0, 0), sf::Vector2f(20, 15), LAYOUT_PLACEMENT::LP_TOP_RIGHT);

	//base->children[0]->children
	test_layout->getLayouts()[0]->addLayout(sf::FloatRect(0, 0, 440, 40), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED);

	//base->children[1]->children
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 140, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 140, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 140, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 140, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 140, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 30, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_RIGHT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 30, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_RIGHT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 30, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_RIGHT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 30, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_RIGHT);
	test_layout->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 30, 30), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_RIGHT);

	//base->children[2]->children
	test_layout->getLayouts()[2]->addLayout(sf::FloatRect(0, 0, 190, 7), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_CENTERED);
	test_layout->getLayouts()[2]->addLayout(sf::FloatRect(0, 0, 80, 5), sf::Vector2f(0, 0), sf::Vector2f(10, 0), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[2]->addLayout(sf::FloatRect(0, 0, 80, 5), sf::Vector2f(0, 0), sf::Vector2f(10, 0), LAYOUT_PLACEMENT::LP_TOP_RIGHT);

	//base->children[3]->children
	test_layout->getLayouts()[3]->addLayout(sf::FloatRect(0, 0, 190, 7), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_CENTERED);
	test_layout->getLayouts()[3]->addLayout(sf::FloatRect(0, 0, 80, 5), sf::Vector2f(0, 0), sf::Vector2f(10, 0), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[3]->addLayout(sf::FloatRect(0, 0, 80, 5), sf::Vector2f(0, 0), sf::Vector2f(10, 0), LAYOUT_PLACEMENT::LP_TOP_RIGHT);

	//base->children[4]->children
	test_layout->getLayouts()[4]->addLayout(sf::FloatRect(0, 0, 190, 7), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_CENTERED);
	test_layout->getLayouts()[4]->addLayout(sf::FloatRect(0, 0, 80, 5), sf::Vector2f(0, 0), sf::Vector2f(10, 0), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[4]->addLayout(sf::FloatRect(0, 0, 80, 5), sf::Vector2f(0, 0), sf::Vector2f(10, 0), LAYOUT_PLACEMENT::LP_TOP_RIGHT);

	//base->children[5]->children
	test_layout->getLayouts()[5]->addLayout(sf::FloatRect(0, 0, 190, 7), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_TOP_CENTERED);
	test_layout->getLayouts()[5]->addLayout(sf::FloatRect(0, 0, 80, 5), sf::Vector2f(0, 0), sf::Vector2f(10, 0), LAYOUT_PLACEMENT::LP_TOP_LEFT);
	test_layout->getLayouts()[5]->addLayout(sf::FloatRect(0, 0, 80, 5), sf::Vector2f(0, 0), sf::Vector2f(10, 0), LAYOUT_PLACEMENT::LP_TOP_RIGHT);

	//base->children[1]->children[0]->child
	test_layout->getLayouts()[1]->getLayouts()[0]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[0]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[0]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[0]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);

	//base->children[1]->children[1]->child
	test_layout->getLayouts()[1]->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[1]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);

	//base->children[1]->children[2]->child
	test_layout->getLayouts()[1]->getLayouts()[2]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[2]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[2]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[2]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);

	//base->children[1]->children[3]->child
	test_layout->getLayouts()[1]->getLayouts()[3]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[3]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[3]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[3]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);

	//base->children[1]->children[4]->child
	test_layout->getLayouts()[1]->getLayouts()[4]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[4]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[4]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	test_layout->getLayouts()[1]->getLayouts()[4]->addLayout(sf::FloatRect(0, 0, 15, 10), sf::Vector2f(0, 0), sf::Vector2f(10, 10), LAYOUT_PLACEMENT::LP_CENTERED_LEFT);
	
	//place them all
	test_layout->placeLayouts();

	//this layout will be fixed and adjustment to be in the users view
	test_layout->setFixedToView(true);

	//add a test-button
	//Button* btn = new Button(10, sf::Text::Style::Bold, sf::Color::Red, std::string("hello"), { 10,10 }, { 300,-100 });
	//test_layout->addObject(btn);

	return test_layout;
}