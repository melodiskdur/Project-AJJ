#include "Button.h"


Button::Button(int char_size, sf::Text::Style text_style, sf::Color text_color, std::string text_string, sf::Vector2f button_size, sf::Vector2f button_pos)
{
	//Set button size
	this->size = button_size;

	//Set button position
	this->world_position = button_pos;

	//Set class id
	this->class_id = "button";

	//Load font
	loadFont("../Project_AJJ/assets/ayar_takhu.ttf");

	//Create text
	setText(char_size, text_style, text_color, text_string);

	//Center the text on the button
	centerText();

	Frame new_frame;
	new_frame.region_name = "MenuButtons";
	new_frame.texture_id = TEXTURE_ID::IDLE;
	new_frame.frame_index = 0;
	new_frame.duration = 0;
	this->setFrame(new_frame);
}

Button::~Button()
{

}

void Button::loadFont(std::string file_path)
{
	//Load desired font
	if (!this->button_font.loadFromFile(file_path))
	{
		std::cout << "ERROR: TestButton::loadFont(std::string file_path), could not load font" << std::endl;
	}
}

void Button::setText(int char_size, sf::Text::Style text_style, sf::Color text_color, std::string text_string)
{
	//Set character size
	this->button_text.setCharacterSize(char_size);

	//Set string style (dold, regular, thin)
	this->button_text.setStyle(text_style);

	//Set text string
	this->button_text.setString(text_string);

	//Set text string color
	this->button_text.setFillColor(text_color);

	//Set the font of the text string
	this->button_text.setFont(this->button_font);
}

void Button::setTextString(std::string text_string) 
{
	//Set the font of the text string
	this->button_text.setString(text_string);
}

void Button::centerText()
{
	//Center the text on the button
	sf::FloatRect bounds = this->button_text.getLocalBounds();
	this->button_text.setPosition(sf::Vector2f(this->world_position.x - ((bounds.width - this->size.x) / 2) , this->world_position.y + ((this->size.y - 1.7f*bounds.height) / 2)));
}