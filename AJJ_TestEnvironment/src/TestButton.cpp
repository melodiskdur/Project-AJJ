#include "../includes/TestButton.h"

Button* TestButton::createTestButton(sf::Vector2f size, Action* action, bool on_fixated_layer, sf::String text)
{
	/*/////////////----CREATE BUTTON----//////////////*/
	Button* btn = new Button({ 0,0 }, size);

	/*/////////////----SET LAYER FIXATION----//////////////*/
	btn->setOnFixatedLayer(on_fixated_layer);

	/*/////////////----ADD FRAMES----//////////////*/
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

	/*/////////////----ADD ACTION----//////////////*/
	btn->addAction(action);

	/*/////////////----SET TEXT----//////////////*/
	btn->loadFont("../Project_AJJ/assets/ayar_takhu.ttf");
	btn->setText(size.x / 8, sf::Text::Style::Bold, sf::Color::White, text);

	return btn;
}
