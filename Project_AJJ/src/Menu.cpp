#include "Menu.h"

Menu::Menu(float width, float height)
{
	//Load desired menu font
	if (!menu_font.loadFromFile("../Project_AJJ/assets/ayar_takhu.ttf"))
	{
		std::cout << "ERROR: Menu::draw(sf::RenderWindow& menu_window), could not load font" << std::endl;
	}

	//Set focused alternative
	focused_alternative = MENU_ALTERNATIVES::PLAY_GAME;

	//Set active alternative (always menu at the beginning)
	active_alternative = MENU_ALTERNATIVES::MAIN_MENU;

	//Create menu alternative "PLAY"
	text_alternative[MENU_ALTERNATIVES::PLAY_GAME].setCharacterSize(30);
	text_alternative[MENU_ALTERNATIVES::PLAY_GAME].setStyle(sf::Text::Bold);
	text_alternative[MENU_ALTERNATIVES::PLAY_GAME].setFillColor(sf::Color::Red);
	text_alternative[MENU_ALTERNATIVES::PLAY_GAME].setString("PLAY");
	text_alternative[MENU_ALTERNATIVES::PLAY_GAME].setFont(menu_font);
	sf::FloatRect pg_textbounds = text_alternative[MENU_ALTERNATIVES::PLAY_GAME].getLocalBounds();
	text_alternative[MENU_ALTERNATIVES::PLAY_GAME].setPosition(sf::Vector2f((width / 2) - pg_textbounds.width / 2, height / (MAXIMUM_NUM_ALTERNATIVES + 1) * 1));

	//Create menu alternative "ABOUT"
	text_alternative[MENU_ALTERNATIVES::ABOUT].setCharacterSize(30);
	text_alternative[MENU_ALTERNATIVES::ABOUT].setStyle(sf::Text::Bold);
	text_alternative[MENU_ALTERNATIVES::ABOUT].setFillColor(sf::Color::White);
	text_alternative[MENU_ALTERNATIVES::ABOUT].setString("ABOUT");
	text_alternative[MENU_ALTERNATIVES::ABOUT].setFont(menu_font);
	sf::FloatRect a_textbounds = text_alternative[MENU_ALTERNATIVES::ABOUT].getLocalBounds();
	text_alternative[MENU_ALTERNATIVES::ABOUT].setPosition(sf::Vector2f((width / 2) - a_textbounds.width / 2, height / (MAXIMUM_NUM_ALTERNATIVES + 1) * 2));

	//Create menu alternative "OPTIONS"
	text_alternative[MENU_ALTERNATIVES::OPTIONS].setCharacterSize(30);
	text_alternative[MENU_ALTERNATIVES::OPTIONS].setStyle(sf::Text::Bold);
	text_alternative[MENU_ALTERNATIVES::OPTIONS].setFillColor(sf::Color::White);
	text_alternative[MENU_ALTERNATIVES::OPTIONS].setString("OPTIONS");
	text_alternative[MENU_ALTERNATIVES::OPTIONS].setFont(menu_font);
	sf::FloatRect o_textbounds = text_alternative[MENU_ALTERNATIVES::OPTIONS].getLocalBounds();
	text_alternative[MENU_ALTERNATIVES::OPTIONS].setPosition(sf::Vector2f((width / 2) - o_textbounds.width / 2, height / (MAXIMUM_NUM_ALTERNATIVES + 1) * 3));
	
	//Create menu alternative "MAP EDITOR"
	text_alternative[MENU_ALTERNATIVES::MAP_EDITOR].setCharacterSize(30);
	text_alternative[MENU_ALTERNATIVES::MAP_EDITOR].setStyle(sf::Text::Bold);
	text_alternative[MENU_ALTERNATIVES::MAP_EDITOR].setFillColor(sf::Color::White);
	text_alternative[MENU_ALTERNATIVES::MAP_EDITOR].setString("MAP EDITOR");
	text_alternative[MENU_ALTERNATIVES::MAP_EDITOR].setFont(menu_font);
	sf::FloatRect me_textbounds = text_alternative[MENU_ALTERNATIVES::MAP_EDITOR].getLocalBounds();
	text_alternative[MENU_ALTERNATIVES::MAP_EDITOR].setPosition(sf::Vector2f((width / 2) - me_textbounds.width / 2, height / (MAXIMUM_NUM_ALTERNATIVES + 1) * 4));

}

Menu::~Menu()
{

}

void Menu::draw(ExtendedRenderWindow &menu_window)
{
	for (int i = 0; i < MAXIMUM_NUM_ALTERNATIVES; i++)
	{
		menu_window.draw(text_alternative[i]);
	}
	
}

void Menu::setFocusedMenuAlternative(MENU_ALTERNATIVES new_foc_alt)
{
	//set fillcolor to non focused color (white)
	this->text_alternative[this->focused_alternative].setFillColor(sf::Color::White);

	//update focused alternative
	this->focused_alternative = new_foc_alt;

	//set fillcolor to focused color (red)
	this->text_alternative[new_foc_alt].setFillColor(sf::Color::Red);
	
}
