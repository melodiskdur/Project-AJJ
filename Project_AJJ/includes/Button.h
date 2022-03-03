#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Object.h"
#include "Action.h"

/*Button class*/

//the type pf trigger the button will respond to
//CLICKED = the button press and release occur on the button. The trigger occur on the release
//PRESSED = the button is pressed, no release neccessary. The trigger occur on the press
//HOVERED = the button is hovered. No press or release necessary. The trigger occur on the hover
//RELEASED = the release occur on the button. No press necessary. The trigger occur on the release
//PRESSED_HOVER = something
enum class TRIGGER_TYPE
{
	CLICKED,
	PRESSED,
	HOVERED,
	RELEASED,
	PRESSED_HOVER
};

class Button :
	public Object
{
public:
	/*Constructors*/
	Button(sf::Vector2f button_pos, sf::Vector2f button_size,  std::string text_string, int char_size, sf::Text::Style text_style, sf::Color text_color);
	Button(sf::Vector2f button_pos, sf::Vector2f button_size);

	/*Destructor*/
	~Button();

	/*Getters*/
	sf::Text getText() { return this->button_text; }
	Frame getDefaultFrame() { return this->default_frame; }
	Frame getHoverFrame() { return this->hover_frame; }
	Frame getPressedFrame() { return this->pressed_frame; }
	bool getOnFixatedLayer() { return this->on_fixated_layer; }
	TRIGGER_TYPE getTriggerType() { return this->trigger_type; }

	/*Setters*/
	//set the sf::Text for this button
	void setText(sf::Text button_text) { this->button_text = button_text; }
	//set the sf::Text for this button
	void setText(int char_size, sf::Text::Style text_style, sf::Color text_color, std::string text_string);
	//set only the text-string
	void setTextString(std::string text_string);

	//set the frame that will occur when the button is in its default state(not hovered or pressed)
	void setDefaultFrame(Frame default_frame) { this->default_frame = default_frame; }
	//set the frame that will occur when the button is hovered
	void setHoverFrame(Frame hover_frame) { this->hover_frame = hover_frame; }
	//set the frame that will occur when the button is pressed
	void setPressedFrame(Frame pressed_frame) { this->pressed_frame = pressed_frame; }
	void setOnFixatedLayer(bool b) { this->on_fixated_layer = b; }
	void setTriggerType(TRIGGER_TYPE trigger_type) { this->trigger_type = trigger_type; }

	/*Others*/
	//Centers the text on the button
	void centerText();
	void updateFrame(bool btn_pressed, bool btn_hovered);
	
	/*Temporary*/
	//TEMPORARY (resource/textloader/something will be added)
	void loadFont(std::string file_path);

private:
	//Parameters	
	sf::Font button_font;									//font for the text
	sf::Text button_text;									//text string

	Frame default_frame;									//the default frame
	Frame hover_frame;										//the frame when the cursor is hovering the button
	Frame pressed_frame;									//the frame when the button is pressed

	TRIGGER_TYPE trigger_type = TRIGGER_TYPE::CLICKED;		//the type of trigger that this button will respond to
	bool on_fixated_layer = false;							//if the button is in the real world or on a view-fixated layer

};
