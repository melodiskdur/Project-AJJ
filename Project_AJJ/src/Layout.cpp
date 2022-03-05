#include "Layout.h"

unsigned int Layout::instance_counter = 0;

/*Constructors*/
Layout::Layout()
{
	Layout::instance_counter++;
}

Layout::Layout(sf::FloatRect rect, sf::Vector2f padding, sf::Vector2f margin, LAYOUT_PLACEMENT layout_placement) : Layout()
{
	this->rect = rect;
	this->padding = padding;
	this->margin = margin;
	this->layout_placement = layout_placement;

	//create 4 margin_spaces(top,right,left,bottom)
	this->margin_spaces = std::vector<sf::FloatRect>(4, sf::FloatRect({ this->rect.left, this->rect.top }, { this->rect.width, this->rect.height }) );
}

/*Destructor*/
Layout::~Layout() 
{ 
	Layout::instance_counter--; 
	for (Object* o : this->objects)
		delete o;
	for (Layout* L : this->layouts)
		delete L;
}

/*Getters*/
std::vector<Button*> Layout::getButtons()
{
	std::vector<Button*> buttons;			//buttonvector to be returned
	int no_buttons = 1;						//check
	//loop all layout-objects
	for (auto& obj : this->objects)
	{
		//if it is a button
		if (obj->getClassId() == "button")
		{
			//add object to the button-vector
			buttons.push_back(dynamic_cast<Button*>(obj));
			no_buttons = 0;
		}
	}
	//return vector
	if (no_buttons)
	{
		//std::cout << "ERROR:(std::vector<Button*> Layout::getButtons()), no buttons in the layout" << std::endl;
	}
	return buttons;
}

sf::FloatRect Layout::getTotalRect()
{
	float tw = this->rect.width + 2 * (this->padding.x + this->margin.x);				//total width: layouts width+padding+margin(left/right)
	float th = this->rect.height + 2 * (this->padding.y + this->margin.y);				//total height: layouts height+padding+margin(top/bottom)
	float tpl = this->rect.left - this->padding.x - this->margin.x;						//total position left
	float tpt = this->rect.top - this->padding.y - this->margin.y;						//total position top
	return sf::FloatRect({ tpl , tpt }, { tw , th });									//floatrect for layout pos and size with padding and margins
}

/*Setters*/
void Layout::setBorderSize(sf::Vector2f new_border_size)
{
	//check if the new border_size is acceptable  
	if (new_border_size.x > this->min_size.x / 2 && new_border_size.y > this->min_size.y / 2)
	{
		//set the new border_size
		this->border_size = new_border_size;
	}
}

void Layout::setBorderSizeForAll(sf::Vector2f new_border_size)
{
	//set the new border_size
	setBorderSize(new_border_size);

	//return if there are no layouts
	if (this->num_layouts == 0) return;

	//loop all of the contained layouts inside of this one 
	for (auto& l : this->layouts)
	{
		//use recursion to the set the new border_size for all layouts
		l->setBorderSizeForAll(new_border_size);
	}
}

void Layout::setPosition(sf::Vector2f new_pos)
{
	//update layouts position
	this->rect.left = new_pos.x;
	this->rect.top = new_pos.y;

	//also, update its margin_spaces position
	int index = 0;
	for (auto& ms : this->margin_spaces)
	{
		ms.left = new_pos.x;
		ms.top = new_pos.y;
		this->setMarginSpaceRect(ms, index);
		index++;
	}
}

void Layout::setPositionForAll(sf::Vector2f new_pos)
{
	sf::Vector2f move_dist = sf::Vector2f(new_pos.x - this->rect.left, new_pos.y - this->rect.top);		//the distance this layout will be moved

	//set the new position
	setPosition(new_pos);

	for (auto& obj : this->objects)
	{
		obj->setWorldPosition({ obj->getWorldPosition().x + move_dist.x, obj->getWorldPosition().y + move_dist.y });
	}

	//finally, use recursion to move all of the layouts inside of this one
	for (auto& child_layout : this->layouts)
	{
		child_layout->setPositionForAll({ child_layout->getPosition().x + move_dist.x, child_layout->getPosition().y + move_dist.y });
	}

}

void Layout::move(sf::Vector2f move_dist)
{
	//add to the current position i.e. move
	setPosition({ getPosition().x + move_dist.x, getPosition().y + move_dist.y });
}

void Layout::moveForAll(sf::Vector2f move_dist)
{
	move(move_dist);

	//return if there are no layouts
	if (this->num_layouts == 0) return;

	//finally, use recursion to move all of the layouts inside of this one
	for (auto& child_layout : this->layouts)
	{
		child_layout->moveForAll(move_dist);
	}

}

void Layout::setTextureNameForAll(sf::String name)
{
	setTextureName(name);

	//return if there are no layouts
	if (this->num_layouts == 0) return;

	for (auto& l : this->layouts)
	{
		l->setTextureNameForAll(name);
	}
}

void Layout::setCurrentFrameForAll(Frame frame)
{
	setCurrentFrame(frame);

	//return if there are no layouts
	if (this->num_layouts == 0) return;

	for (auto& l : this->layouts)
	{
		l->setCurrentFrameForAll(frame);
	}
}

void Layout::setEnabledForAll(bool state)
{
	//set state for this layout
	this->setEnabled(state);

	//loop all of the child-layouts
	for (Layout* child_layout : this->getLayouts())
	{
		//recursion
		child_layout->setEnabledForAll(state);
	}

	//stop the recursion
	return;
}


/*Others*/
void Layout::addLayout(Layout* new_layout)
{
	if (!validSize(new_layout->getSize())) return;

	//add layout to child-layout-vector
	this->layouts.push_back(new_layout);

	//increase the number number of layouts variable
	this->num_layouts++;
}

void Layout::addLayout(sf::FloatRect rect, sf::Vector2f padding, sf::Vector2f margin, LAYOUT_PLACEMENT layout_placement)
{
	if (!validSize({ rect.width,rect.height })) return;
	
	//create new layout
	Layout* new_layout = new Layout(rect,padding,margin,layout_placement);

	//set parent
	new_layout->parent_layout = this;

	//add to list of child-layouts
	this->layouts.push_back(new_layout);

	//increase the number of layouts variable
	this->num_layouts++;
}

void Layout::addObject(Object* new_object)
{
	//add object to object-vector
	this->objects.push_back(new_object);

	//if it is a button, increase the number of button variable
	if (new_object->getClassId() == "button")
	{
		this->num_buttons++;
	}

	//increase the number of overall objects
	this->num_objects++;
}

void Layout::placeLayouts()
{
	//if this is the base-layout
	if (this->parent_layout == nullptr)
	{
		//DO SOMETHING...maybe
	}

	//place all of the available objects
	this->placeLayoutObjects();

	//loop all "child"-layouts to be placed inside of this layout
	for (auto& l : this->layouts)
	{
		//set ideal positioning depending on the layout-placement
		l->setLayoutIdealPosition();

		//calculate and set the bestplacement for the layout within th margin_spaces
		l->setBestLayoutPlacement();

		//update the parent-layouts margin_spaces to match with the newly placed layout
		//the parent_layout is in this case "this"
		l->updateParentMarginSpaces(l->getTotalRect(),this);

		//use recursion to place the layouts inside of layout l
		//this way, we place every layout available inside of the whole "base"-layout
		l->placeLayouts();
	}
}

void Layout::placeLayoutObjects()
{
	//loop all of the objects and place them
	for (auto& obj : this->objects)
	{
		//set the ideal object_placement inside of this layout
		this->setObjectIdealPosition(obj);

		//set the best placement along what is available 
		//from the margin_spaces and is closest to the ideal position
		this->setBestObjectPlacement(obj);

		//update all of the margin_spaces so that the placed object dont overlap
		this->updateParentMarginSpaces(obj->getFloatRect(), this);
	}
}

void Layout::setLayoutIdealPosition()
{
	sf::FloatRect pl_rect = this->parent_layout->getRect();			//the rect for the parent_layout

	//TOP
	if (this->layout_placement == LAYOUT_PLACEMENT::TOP_CENTERED)
	{
		this->setPosition({ pl_rect.left + (pl_rect.width / 2) - (this->rect.width / 2), pl_rect.top });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::TOP_LEFT)
	{
		this->setPosition({ pl_rect.left,pl_rect.top });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::TOP_RIGHT)
	{
		this->setPosition({ pl_rect.left + pl_rect.width - this->rect.width , pl_rect.top });
	}
	//BOTTOM
	else if (this->layout_placement == LAYOUT_PLACEMENT::BOTTOM_CENTERED)
	{
		this->setPosition({ pl_rect.left + (pl_rect.width / 2) - (this->rect.width / 2) , pl_rect.top + pl_rect.height - this->rect.height });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::BOTTOM_LEFT)
	{
		this->setPosition({ pl_rect.left , pl_rect.top + pl_rect.height - this->rect.height });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::BOTTOM_RIGHT)
	{
		this->setPosition({ pl_rect.left + pl_rect.width - this->rect.width , pl_rect.top + pl_rect.height - this->rect.height });
	}
	//CENTERED
	else if (this->layout_placement == LAYOUT_PLACEMENT::CENTERED_LEFT)
	{
		this->setPosition({ pl_rect.left , pl_rect.top + (pl_rect.height / 2) - (this->rect.height / 2) });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::CENTERED_RIGHT)
	{
		this->setPosition({ pl_rect.left + pl_rect.width - this->rect.width , pl_rect.top + (pl_rect.height / 2) - (this->rect.height / 2) });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::CENTERED)
	{
		this->setPosition({ pl_rect.left + (pl_rect.width / 2) - (this->rect.width / 2) , pl_rect.top + (pl_rect.height / 2) - (this->rect.height / 2) });
	}
	//CUSTOM and NONE
	else if (this->layout_placement == LAYOUT_PLACEMENT::CUSTOM)
	{
		//continue with the given position
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::NONE)
	{
		//no position ha been given
		//could be useful when implementing a drag and drop i.e. in hand/not placed
	}
}

void Layout::setObjectIdealPosition(Object * obj)
{
	if (!(this->num_objects > 0)) return;

	sf::FloatRect pl_rect = this->getRect();			//the rect for the parent_layout
	sf::FloatRect obj_rect = obj->getFloatRect();

	//TOP
	if (this->objects_placement == LAYOUT_PLACEMENT::TOP_CENTERED)
	{
		obj->setWorldPosition({ pl_rect.left + (pl_rect.width / 2) - (obj_rect.width / 2), pl_rect.top });
	}
	else if (this->objects_placement == LAYOUT_PLACEMENT::TOP_LEFT)
	{
		obj->setWorldPosition({ pl_rect.left,pl_rect.top });
	}
	else if (this->objects_placement == LAYOUT_PLACEMENT::TOP_RIGHT)
	{
		obj->setWorldPosition({ pl_rect.left + pl_rect.width - obj_rect.width , pl_rect.top });
	}
	//BOTTOM
	else if (this->objects_placement == LAYOUT_PLACEMENT::BOTTOM_CENTERED)
	{
		obj->setWorldPosition({ pl_rect.left + (pl_rect.width / 2) - (obj_rect.width / 2) , pl_rect.top + pl_rect.height - obj_rect.height });
	}
	else if (this->objects_placement == LAYOUT_PLACEMENT::BOTTOM_LEFT)
	{
		obj->setWorldPosition({ pl_rect.left , pl_rect.top + pl_rect.height - this->rect.height });
	}
	else if (this->objects_placement == LAYOUT_PLACEMENT::BOTTOM_RIGHT)
	{
		obj->setWorldPosition({ pl_rect.left + pl_rect.width - obj_rect.width , pl_rect.top + pl_rect.height - obj_rect.height });
	}
	//CENTERED
	else if (this->objects_placement == LAYOUT_PLACEMENT::CENTERED_LEFT)
	{
		obj->setWorldPosition({ pl_rect.left , pl_rect.top + (pl_rect.height / 2) - (obj_rect.height / 2) });
	}
	else if (this->objects_placement == LAYOUT_PLACEMENT::CENTERED_RIGHT)
	{
		obj->setWorldPosition({ pl_rect.left + pl_rect.width - obj_rect.width , pl_rect.top + (pl_rect.height / 2) - (obj_rect.height / 2) });
	}
	else if (this->objects_placement == LAYOUT_PLACEMENT::CENTERED)
	{
		obj->setWorldPosition({ pl_rect.left + (pl_rect.width / 2) - (obj_rect.width / 2) , pl_rect.top + (pl_rect.height / 2) - (obj_rect.height / 2) });
	}
	//CUSTOM and NONE
	else if (this->objects_placement == LAYOUT_PLACEMENT::CUSTOM)
	{
		//continue with the given position
	}
	else if (this->objects_placement == LAYOUT_PLACEMENT::NONE)
	{
		//no position has been given
		//could be useful when implementing a drag and drop i.e. in hand/not placed
	}
}

void Layout::setBestLayoutPlacement()
{
	/*This function calculates the best possible placement for this layout.
	inside of the given margin_spaces. This layout has been given a position,
	in some cases its ideal position. This function then look at all of the available
	margin_spaces that it can be placed inside of and finds the closest possible position*/

	sf::FloatRect layout = this->getTotalRect();								//the whole layout, including padding and margins
	float distance_to_ideal_pos = 9999999;										//sentinel value for the distance to the ideal placement
	sf::Vector2f adjusted_placement;											//the final adjusted postion. This layouts position will be set to this value at the end

	for (auto& ms : this->parent_layout->getMarginSpaces())
	{
		//check if the layout can fit inside the margin_space
		if (possibleFit(ms, layout))
		{
			//calculate adjusted new position inside of the margin_space
			sf::Vector2f new_pos = fitInside(ms, layout);

			//calculate the distance to the ideal position
			float d_new = distance(new_pos, { layout.left, layout.top });

			//if that placement currently is the best option for the layout
			if (d_new < distance_to_ideal_pos)
			{
				//update adjusted placement position
				distance_to_ideal_pos = d_new;
				adjusted_placement = new_pos;
			}
		}

	}

	this->setPosition(correctPosition(adjusted_placement));
}

void Layout::setBestObjectPlacement(Object* obj)
{
	sf::FloatRect obj_rect = obj->getFloatRect();								//the whole layout, including padding and margins
	float distance_to_ideal_pos = 9999999;										//sentinel value for the distance to the ideal placement
	sf::Vector2f adjusted_placement;											//the final adjusted postion. This layouts position will be set to this value at the end

	for (auto& ms : this->getMarginSpaces())
	{
		//check if the layout can fit inside the margin_space
		if (possibleFit(ms, obj_rect))
		{
			//calculate adjusted new position inside of the margin_space
			sf::Vector2f new_pos = fitInside(ms, obj_rect);

			//calculate the distance to the ideal position
			float d_new = distance(new_pos, { obj_rect.left, obj_rect.top });

			//if that placement currently is the best option for the layout
			if (d_new < distance_to_ideal_pos)
			{
				//update adjusted placement position
				distance_to_ideal_pos = d_new;
				adjusted_placement = new_pos;
			}
		}

	}

	obj->setWorldPosition(adjusted_placement);
}

void Layout::updateParentMarginSpaces(sf::FloatRect rect, Layout * layout)
{
	int index = 0;
	//index % 4 = 0 -> treat the margin as a top_margin. DECREASE HEIGHT
	//index % 4 = 1 -> treat the margin as a right_margin. INCREASE LEFT-POSITION AND DECREASE WIDTH
	//index % 4 = 2 -> treat the margin as a bottom_margin. INCREASE TOP-POSITION AND DECREASE HEIGHT
	//index % 4 = 3 -> treat the margin as a left_margin. DECREASE WIDTH

	//loop all of the parent_layout's margin_spaces
	for (auto& ms : layout->getMarginSpaces())
	{
		
		sf::FloatRect instersect_area;
		if (ms.intersects(rect, instersect_area))
		{
			//positive diff: rect's left/right side is to the right of margin_space's left/right side.
			float right_diff = (rect.left + rect.width) - (ms.left + ms.width);
			float left_diff = rect.left - ms.left;

			//positive diff: rect's top/bottom side is below the margin_space's top/bottom side.
			float bottom_diff = (rect.top + rect.height) - (ms.top + ms.height);
			float top_diff = rect.top - ms.top;

			int no_adjustment = 1;								//check if an adjustment were made

			//make an adjustment to margin_space
			
			//top_margin
			if (index % 4 == 0)
			{
				//check if top-adjustment is possible
				if (top_diff > 0)
				{
					ms.height = top_diff;
					no_adjustment = 0;
				}
				//if not, choose another type of adjustment
				else
				{
					/*For top_margin, left adjustment is favored, then right and last bottom.*/

					if (left_diff > 0)
					{
						ms.width = left_diff;
						no_adjustment = 0;
					}
					else if (right_diff < 0)
					{
						ms.left = rect.left + rect.width;
						ms.width = -right_diff;
						no_adjustment = 0;
					}
					else if (bottom_diff < 0)
					{
						ms.top = rect.top + rect.height;
						ms.height = -bottom_diff;
						no_adjustment = 0;
					}
				}
			}

			//right_margin
			else if (index % 4 == 1)
			{
				//check if right-adjustment is possible
				if (right_diff < 0)
				{
					ms.left = rect.left + rect.width;
					ms.width = -right_diff;
					no_adjustment = 0;
				}
				//if not, choose another type of adjustment
				else
				{
					/*For right_margin, bottom adjustment is favored, then top and last left.*/

					if (bottom_diff < 0)
					{
						ms.top = rect.top + rect.height;
						ms.height = -bottom_diff;
						no_adjustment = 0;
					}
					else if (top_diff > 0)
					{
						ms.height = top_diff;
						no_adjustment = 0;
					}
					else if (left_diff > 0)
					{
						ms.width = left_diff;
						no_adjustment = 0;
					}
				}
			}

			//bottom_margin
			else if (index % 4 == 2)
			{
				//check if bottom-adjustment is possible
				if (bottom_diff < 0)
				{
					ms.top = rect.top + rect.height;
					ms.height = -bottom_diff;
					no_adjustment = 0;
				}
				//if not, choose another type of adjustment
				else
				{
					/*For bottom_margin, right adjustment is favored, then left and last top.*/

					if (right_diff < 0)
					{
						ms.left = rect.left + rect.width;
						ms.width = -right_diff;
						no_adjustment = 0;
					}
					else if (left_diff > 0)
					{
						ms.width = left_diff;
						no_adjustment = 0;
					}
					else if (top_diff > 0)
					{
						ms.height = top_diff;
						no_adjustment = 0;
					}
				}
			}

			//left_margin
			else if (index % 4 == 3)
			{
				//check if left-adjustment is possible
				if (left_diff > 0)
				{
					ms.width = left_diff;
					no_adjustment = 0;
				}
				//if not, choose another type of adjustment
				else
				{
					/*For left_margin, top adjustment is favored, then bottom and last right.*/


					if (top_diff > 0)
					{
						ms.height = top_diff;
						no_adjustment = 0;
					}
					else if (bottom_diff < 0)
					{
						ms.top = rect.top + rect.height;
						ms.height = -bottom_diff;
						no_adjustment = 0;
					}
					else if (right_diff < 0)
					{
						ms.left = rect.left + rect.width;
						ms.width = -right_diff;
						no_adjustment = 0;
					}
				}
			}
		
			// Update the margin_rect's floatrect. 
			// The ms is in this case not updated by the operations above. This step is necessary
			layout->setMarginSpaceRect(ms,index);
		}

		index++;
	}
}

void Layout::resetMarginSpaces()
{
	//resturn if this layout dont contain any other layouts
	//if so, the margin_spaces havent changed
	if (this->num_layouts == 0 && this->num_objects == 0) return;

	//loop all margin_spaces and reset them
	for (auto& ms : this->margin_spaces)
	{
		ms = this->rect;
	}
	//loop all of the contained layouts inside of this one 
	for (auto& l : this->layouts)
	{
		//use recursion to reset every other margin_space
		l->resetMarginSpaces();
	}
}


/*Help-mehthods*/
float Layout::distance(sf::Vector2f s, sf::Vector2f d)
{
	/*This function calculates the distnace from source-point to
	destination-point.*/

	// s = source
	// d = destination

	// calculating distance
	return static_cast<float>(sqrt(pow(d.x - s.x, 2) + pow(d.y - s.y, 2) * 1.0));
}

bool Layout::possibleFit(sf::FloatRect margin_space, sf::FloatRect rect)
{
	/*This function returns true if rect could fit inside of margin_space,
	otherwise false. Margin_space is seen as stationary and rect
	can be moved around freely.*/

	//rect = rect to be placed in margin_space
	//margin_space = container/available space that the layout can be placed inside of

	if (rect.width > margin_space.width || rect.height > margin_space.height)
	{
		//not possible to place layout inside of margin_space
		return false;
	}

	return true;
}

sf::Vector2f Layout::fitInside(sf::FloatRect margin_space, sf::FloatRect rect)
{
	/*This function calculate the position of rect so that it would
	fit inside of margin_space and returns the result. Margin_space is seen
	as stationary and rect can be moved around freely. If the layout
	is already placed inside of margin_space, nothing is done. If rect
	cant physically fit inside of margin_space, an empty list is returned.*/

	//rect = rect to be placed in margin_space
	//margin_space = container/available space that the rect can be placed inside of

	//first, check if the fit is possible
	if (!possibleFit(margin_space, rect))
	{
		//if not, nothing can be done. Return empty list, meaning no position could be found
		return {};
	}

	//positive diff: rect's left/right side is to the right of margin_space's left/right side.
	float right_diff = (rect.left + rect.width) - (margin_space.left + margin_space.width);
	float left_diff = rect.left - margin_space.left;

	//positive diff: rect's top/bottom side is below the margin_space's top/bottom side.
	float bottom_diff = (rect.top + rect.height) - (margin_space.top + margin_space.height);
	float top_diff = rect.top - margin_space.top;

	/*LOGIC-TABLE LEFT/RIGHT OR TOP/BOTTOM
	* we only need to make an adjustment if both diffs are negative
	* or if they are both positive. Else, its already inside of the rect.
	________________________________________________________
	| top/left_diff | bottom/right_diff	|		action		|
	|_______________________________________________________|
	|	  -			|		-			|	top/left_adjust	|
	|	  +			|		-			|	    inside		|
	|	  -			|		+			|	  IMPOSSIBLE	|
	|	  +			|		+			|bottom/right_adjust|
	|	  0			|		0			|	    inside		|
	|	  0			|		-			|	    inside		|
	|	  0			|		+			|	  IMPOSSIBLE	|
	|	  -			|		0			|	  IMPOSSIBLE	|
	|	  +			|		0			|	    inside		|
	|_______________________________________________________|
	*/

	//top_adjust
	if (top_diff < 0 && bottom_diff < 0)
	{
		rect.top -= top_diff;
	}
	//bottom_adjust
	else if (top_diff > 0 && bottom_diff > 0)
	{
		rect.top -= bottom_diff;
	}
	//left_adjust
	if (left_diff < 0 && right_diff < 0)
	{
		rect.left -= left_diff;
	}
	//right_adjust
	else if (left_diff > 0 && right_diff > 0)
	{
		rect.left -= right_diff;
	}

	return { rect.left, rect.top };

}

sf::Vector2f Layout::correctPosition(sf::Vector2f total_rect_pos)
{
	//correct the position
	total_rect_pos.x += (this->padding.x + this->margin.x);
	total_rect_pos.y += (this->padding.y + this->margin.y);

	//return the corrected position
	return total_rect_pos;
}

bool Layout::validSize(sf::Vector2f size)
{
	if (size.x < this->min_size.x || size.y < this->min_size.y)
	{
		return false;
	}
	return true;
}