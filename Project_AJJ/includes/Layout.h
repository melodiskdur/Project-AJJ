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
* this is used to make the placement of layouts inside of other layouts simpler
* If this functionality is not wanted, set LP_NONE and specify a position.
*/

//placement of the layout in relation to the window or parent-layout
enum class LAYOUT_PLACEMENT
{
	TOP_CENTERED,
	BOTTOM_CENTERED,
	CENTERED_LEFT,
	CENTERED_RIGHT,
	TOP_LEFT,
	TOP_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_RIGHT,
	CENTERED,
	CUSTOM,
	NONE
};

class Layout
{
public:
	/*Constructors*/
	Layout();
	Layout(sf::FloatRect rect, sf::Vector2f padding, sf::Vector2f margin, LAYOUT_PLACEMENT layout_placement);

	/*Destructor*/
	virtual ~Layout();

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
	
	//returns the name of the texture that this layout use
	sf::String getTextureName() { return this->texture_name; };
	//returns the current frame
	Frame getCurrentFrame() { return this->current_frame; };

	//returns the layouts parent-layout. This layout is contained inside of its parent
	Layout* getParentLayout() { return this->parent_layout; }

	//returns the minimum size for the layout
	sf::Vector2f getMinSize() { return this->min_size; }
	//returns the current border_size for the layout. Cant exceed half of the minimum size
	sf::Vector2f getBorderSize() { return this->border_size; }
	//returns the state of the layout and if its fixated to a view or not.
	bool getfixedToView() { return this->fixed_to_view; }

	int getNumLayouts() { return this->num_layouts; }
	int getNumObjects() { return this->num_objects; }
	int getNumButtons() { return this->num_buttons; }
	bool getEnabled() { return this->enabled; }


	/*Setters*/
	void setText(sf::Text new_text) { this->text = new_text; }
	void setRect(sf::FloatRect rect) { this->rect = rect; }
	void setSize(sf::Vector2f size ) { this->rect.width = size.x, this->rect.height = size.y; }
	void setPosition(sf::Vector2f new_pos);
	void setPositionForAll(sf::Vector2f new_pos);
	void move(sf::Vector2f move_dist);
	void moveForAll(sf::Vector2f move_dist);
	void setPadding(sf::Vector2f padding) { this->padding = padding; }
	void setMargin(sf::Vector2f margin) { this->margin = margin; }
	void setMarginSpaceRect(sf::FloatRect new_margin_space, int index) { this->margin_spaces[index] = new_margin_space; }
	void setTextureName(sf::String name) { this->texture_name = name; };
	void setTextureNameForAll(sf::String name);
	void setCurrentFrame(Frame frame) { this->current_frame = frame; };
	void setCurrentFrameForAll(Frame frame);
	void setParentLayout(Layout* parent_layout) { this->parent_layout = parent_layout; }
	void setFixedToView(bool state) { this->fixed_to_view = state; }
	void setBorderSize(sf::Vector2f border_size);
	void setBorderSizeForAll(sf::Vector2f new_border_size);
	void setLayoutPlacement(LAYOUT_PLACEMENT lyt_plc) { this->objects_placement = lyt_plc; }
	void setObjectsPlacement(LAYOUT_PLACEMENT objs_plc) { this->objects_placement = objs_plc; }
	void setEnabled(bool state) { this->enabled = state; }
	void setEnabledForAll(bool state);

	/*Others*/
	//add a another layout inside of this one i.e. add it to the Layout*-vector
	void addLayout(Layout* new_layout);
	//create and add another layout inside of this one i.e. add it to the Layout*-vector
	void addLayout(sf::FloatRect rect, sf::Vector2f padding, sf::Vector2f margin, LAYOUT_PLACEMENT layout_placement);
	//add an object inside of the layout(button,other-stuff,etc.)
	void addObject(Object* new_object);

	//adjust the layout_placements. A combination of setIdealLayoutPosition(), 
	//setBestLayoutPlacement() and updateParentMarginSpaces(). Uses recursion to place
	//all of the "child"-layouts contained inside
	void placeLayouts();

	//place the objects inside of this layout
	void placeLayoutObjects();

	//set the ideal placement of this layout inside of its parent-layout
	void setLayoutIdealPosition();
	//calculate the best placement of this layout in the available margin_spaces
	void setBestLayoutPlacement();

	//set the ideal placement for the object inside of this layout
	void setObjectIdealPosition(Object* object);
	//calculate the best placement of the object in the available margin_spaces
	void setBestObjectPlacement(Object* obj);

	//correct/update the available margin_spaces in layout after placing rect(layout or object)
	void updateParentMarginSpaces(sf::FloatRect rect, Layout * layout);

	//resets all of the margin_spaces for all of the layouts
	//this step is needed before placeLayouts() if it is run again
	void resetMarginSpaces();

	//sometimes calculations get easier if we use the "total_rect" of the layout
	//this rect includes the padding and margin. The result of the calulations
	//often result in a position for this rect, which then nedd to be adjusted
	//so that it doesnt include the layouts padding and margin. This function does that.
	sf::Vector2f correctPosition(sf::Vector2f total_rect_pos);

	//calculate the position so that rect fits inside of margin_space
	sf::Vector2f fitInside(sf::FloatRect margin_space, sf::FloatRect rect);
	//if the rect could fit inside of margin_space (true/false)
	bool possibleFit(sf::FloatRect margin_space, sf::FloatRect rect);
	//calculates the distance between two xy-points
	float distance(sf::Vector2f s, sf::Vector2f d);
	//check for if the size is a valid size for this layout i.e. that it is larger than the minimum size
	bool validSize(sf::Vector2f size);
	
	static unsigned int instanceCount() { return instance_counter; };
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
	sf::Vector2f border_size = { 4,4 };				//the layouts borders size in pixels, width and height

	std::vector<sf::FloatRect> margin_spaces;		//used to check the remaining space of the layout (top,bottom,left,right margin space)

	bool fixed_to_view = false;						//if the layout is fixed to the users view. If false, no adjustment is needed (rect.x and rect.y specify its location)
	bool enabled = true;							//if we want to draw the layout or not

	//Textures
	sf::String texture_name = "";					//Name of the texture atlas that we want to draw frames of this object from.
	Frame current_frame;							//the current frame for this layout. includes information about texture id, region, frameindex, duration

	LAYOUT_PLACEMENT layout_placement = LAYOUT_PLACEMENT::NONE;			//placement of the layout in relation to the window or parent-layout
	LAYOUT_PLACEMENT objects_placement = LAYOUT_PLACEMENT::TOP_LEFT;	//placement of the objects inside of the layout
	static unsigned int instance_counter;
};

