#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Object.h"

class Button :
	public Object
{
public:
	//Constructor
	Button(int char_size, sf::Text::Style text_style, sf::Color text_color, std::string text_string, sf::Vector2f button_size, sf::Vector2f button_pos);

	//Destructor
	~Button();

	//Getters
	sf::Text getText() { return this->button_text; }

	//Setters
	void setText(sf::Text button_text) { this->button_text = button_text; }
	void setText(int char_size, sf::Text::Style text_style, sf::Color text_color, std::string text_string);
	void setTextString(std::string text_string);

	//Others
	//Temp
	void loadFont(std::string file_path);
	void centerText();

private:
	//Parameters
	sf::Font button_font;
	sf::Text button_text;

};
