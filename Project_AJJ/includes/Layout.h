#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Animation.h"
#include "TextureIds.h"
#include "Button.h"

/*Class for layouts(menus,textbubbles,screen-messages,popups,etc)
* Layouts can contain objects such as buttons...(more will be added)
* It can also contain text and other layouts.
* Each layout has a layout-placement, similar to the placement method used in html
*/

//placement of the layout in relation to the window or parent-layout
enum class LAYOUT_PLACEMENT
{
	LP_TOP_CENTERED,
	LP_BOTTOM_CENTERED,
	LP_CENTERED_LEFT,
	LP_CENTERED_RIGHT,
	LP_TOP_LEFT,
	LP_TOP_RIGHT,
	LP_BOTTOM_LEFT,
	LP_BOTTOM_RIGHT,
	LP_CENTERED,
	LP_CUSTOM,
	LP_NONE
};

class Layout
{
public:
	/*Constructors*/
	Layout();
	Layout(sf::FloatRect rect, sf::Vector2f padding, sf::Vector2f margin, LAYOUT_PLACEMENT layout_placement);

	/*Destructor*/
	~Layout();


	/*Getters*/
	//returns the sf::Text contained inside of the layout
	sf::Text getText() { return this->text; }
	//returns all of the layout-objects
	std::vector<Object*> getObjects() { return this->objects; }	
	//get layout-objects that are buttons
	std::vector<Button*> getButtons();	

	//returns the floatrect(pos and size) of the layout
	sf::FloatRect getRect() { return this->rect; }
	//returns the layouts size(width and height)
	sf::Vector2f getSize() { return { this->rect.width , this->rect.height }; }
	//returns the custom layout placement 
	sf::Vector2f getPosition() { return { this->rect.left , this->rect.top }; }
	//returns the paddind-size around the layout
	sf::Vector2f getPadding() { return this->padding; }
	//returns the margin for the layout i.e. layout-safe-zone
	sf::Vector2f getMargin() { return this->margin; }
	//returns the total rect i.e. includes margins and padding to the original layout-rect
	sf::FloatRect getTotalRect();
	//return the child-layouts
	std::vector<Layout*> getLayouts() { return this->layouts; }
	//get the vector of available marginspaces to place a layout/object inside of
	std::vector<sf::FloatRect> getMarginSpaces() { return this->margin_spaces; }
	//sometimes calculations get easier if we use the "total_rect" of the layout
	//this rect includes the padding and margin. The result of the calulations
	//often result in a position for this rect, which then nedd to be adjusted
	//so that it doesnt include the layouts padding and margin. This function does that.
	sf::Vector2f correctPosition(sf::Vector2f total_rect_pos);

	sf::String getTextureName() { return this->texture_name; };
	TEXTURE_ID getTextureId() { return this->active_texture; };
	Frame getFrame() { return this->current_frame; };

	Layout* getParentLayout() { return this->parent_layout; }

	sf::Vector2f getMinSize() { return this->min_size; }


	/*Setters*/
	void setText(sf::Text new_text) { this->text = new_text; }
	void setRect(sf::FloatRect rect) { this->rect = rect; }
	void setSize(sf::Vector2f size ) { this->rect.width = size.x, this->rect.height = size.y; }
	void setPosition(sf::Vector2f new_pos);
	void setPadding(sf::Vector2f padding) { this->padding = padding; }
	void setMargin(sf::Vector2f margin) { this->margin = margin; }
	void setMarginSpaceRect(sf::FloatRect new_margin_space, int index) { this->margin_spaces[index] = new_margin_space; }

	void setTextureName(sf::String name) { this->texture_name = name; };
	void setTextureId(TEXTURE_ID texture_id) { this->active_texture = texture_id; };
	void setFrame(Frame frame) { this->current_frame = frame; };

	void setParentLayout(Layout* parent_layout) { this->parent_layout = parent_layout; }
	
	

	/*Others*/
	//add a another layout inside of this one i.e. add it to the Layout*-vector
	void addLayout(Layout* new_layout);
	//create and add another layout inside of this one i.e. add it to the Layout*-vector
	void addLayout(sf::FloatRect rect, sf::Vector2f padding, sf::Vector2f margin, LAYOUT_PLACEMENT layout_placement);
	//add an object inside of the layout(button,other-stuff,etc.)
	void addObject(Object* new_object);

	//adjust the layout_placements
	void placeLayouts();
	//set the ideal placement inside of the parent_layout depending on LAYOUT_PLACEMENT
	void setIdealPosition();
	//calculate and return the best placement of this layout in the available margin_spaces
	void setBestLayoutPlacement();
	//correct/update the available margin_spaces after placing a layout
	void updateParentMarginSpaces();

	void resetMarginSpaces();

	//calculate the position so that rect fits inside of margin_space
	sf::Vector2f fitInside(sf::FloatRect margin_space, sf::FloatRect rect);
	//if the rect could fit inside of margin_space (true/false)
	bool possibleFit(sf::FloatRect margin_space, sf::FloatRect rect);
	//calculates the distance between two xy-points
	float distance(sf::Vector2f s, sf::Vector2f d);

	bool validSize(sf::Vector2f size);
	
protected:
	sf::Text text;									//text inside of the layout

	std::vector<Object*> objects;					//layout-objects(buttons, sidescroller, etc.)
	int num_objects = 0;							//the number layout-objects
	int num_buttons = 0;							//number of buttons

	Layout* parent_layout = nullptr;				//the layout outside of this layout
	std::vector<Layout*> layouts;					//layouts inside of this layout
	int num_layouts = 0;							//the number of layouts

	sf::FloatRect rect = { 0,0,0,0 };				//position and size
	sf::Vector2f padding = { 0,0 };					//padding around the layout, left/right and top/bottom
	sf::Vector2f margin = { 0,0 };					//space between other layouts or window-bordes, left/right and top/bottom
	sf::Vector2f min_size = { 4,4 };				//the minimum width and height for a layout

	std::vector<sf::FloatRect> margin_spaces;		//used to check the remaining space of the layout (top,bottom,left,right margin space)

	//Textures
	sf::String texture_name = "";					//Name of the texture atlas that we want to draw frames of this object from.
	TEXTURE_ID active_texture = TEXTURE_ID::NONE;
	Frame current_frame;

	LAYOUT_PLACEMENT layout_placement = LAYOUT_PLACEMENT::LP_NONE;	//placement of the layout in relation to the window or parent-layout
};

