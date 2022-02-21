#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Object.h"
#include "Action.h"


class Button :
	public Object
{
public:
	//Constructor
	Button(sf::Vector2f button_pos, sf::Vector2f button_size,  std::string text_string, int char_size, sf::Text::Style text_style, sf::Color text_color);
	Button(sf::Vector2f button_pos, sf::Vector2f button_size);

	//Destructor
	~Button();

	//Getters
	sf::Text getText() { return this->button_text; }
	Frame getDefaultFrame() { return this->default_frame; }
	Frame getHoverFrame() { return this->hover_frame; }
	Frame getPressedFrame() { return this->pressed_frame; }

	//Setters
	void setText(sf::Text button_text) { this->button_text = button_text; }
	void setText(int char_size, sf::Text::Style text_style, sf::Color text_color, std::string text_string);
	void setTextString(std::string text_string);
	void setDefaultFrame(Frame default_frame) { this->default_frame = default_frame; }
	void setHoverFrame(Frame hover_frame) { this->hover_frame = hover_frame; }
	void setPressedFrame(Frame pressed_frame) { this->pressed_frame = pressed_frame; }

	//Others
	//Temp
	void loadFont(std::string file_path);
	void centerText();

private:
	//Parameters
	sf::Font button_font;
	sf::Text button_text;
	Frame default_frame;
	Frame hover_frame;
	Frame pressed_frame;
	

};
