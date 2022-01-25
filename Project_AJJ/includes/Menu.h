#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "ExtendedRenderWindow.h"

/*Class for handling menus in the program
* MENU_ALTERNATIVES contains all of the menu-alternatives the user can pick from
* The input-handling is made by the programs main controller
* This class can be used for the main menu as well as other ingame menus 
* 
* OBS!!- The constructor is currently hardcoded for the main_menu.
*/

enum MENU_ALTERNATIVES
{
	PLAY_GAME,
	ABOUT,
	OPTIONS,
	MAP_EDITOR,
	MAIN_MENU
};

#define MAXIMUM_NUM_ALTERNATIVES 4

class Menu
{
public:
	//Constructor
	Menu(float width, float height);

	//Destructor
	~Menu();

	//Getters
	MENU_ALTERNATIVES getFocusedMenuAlternative() { return this->focused_alternative; }
	MENU_ALTERNATIVES getActiveMenuAlternative() { return this->active_alternative; }
	sf::Text* getTextAlternatives() { return this->text_alternative; }

	//Setters
	void setFocusedMenuAlternative(MENU_ALTERNATIVES new_foc_alt);
	void setActiveMenuAlternative(MENU_ALTERNATIVES new_act_alt) { this->active_alternative = new_act_alt; }

	//Others
	void draw(ExtendedRenderWindow &menu_window);	//draw all text alternatives

protected:
	//Parameters
	sf::Font menu_font;
	sf::Text text_alternative[MAXIMUM_NUM_ALTERNATIVES];
	MENU_ALTERNATIVES focused_alternative;			//the current highlighted alternative
	MENU_ALTERNATIVES active_alternative;			//the current alternative that has been picked



};

