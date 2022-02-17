#include "Layout.h"

/*Constructors*/
Layout::Layout()
{
	/*
	Button* new_test_button = new Button(20, sf::Text::Bold, sf::Color(178,178,178,255), (std::string)"TEST" , {150,70}, {100,100});
	this->buttons.push_back(new_test_button);*/
}

Layout::Layout(sf::FloatRect rect, sf::Vector2f padding, sf::Vector2f margin, LAYOUT_PLACEMENT layout_placement)
{
	this->rect = rect;
	this->padding = padding;
	this->margin = margin;
	this->layout_placement = layout_placement;

	//create 4 margin_spaces(top,right,left,bottom)
	this->margin_spaces = std::vector<sf::FloatRect>(4, sf::FloatRect({ this->rect.left, this->rect.top }, { this->rect.width, this->rect.height }) );

	Frame new_frame;
	new_frame.region_name = "LayoutBlock";
	new_frame.texture_id = TEXTURE_ID::IDLE;
	new_frame.frame_index = 0;
	new_frame.duration = 0;

	this->current_frame = new_frame;
	this->texture_name = "LayoutBlock";
}

/*Destructor*/
Layout::~Layout()
{

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
		std::cout << "ERROR:(std::vector<Button*> Layout::getButtons()), no buttons in the layout" << std::endl;
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
	//check if this layout contain any layouts. If no layouts exist, return.
	if (this->num_layouts == 0) { return; }

	//loop all "child"-layouts to be placed inside of this layout
	for (auto& l : this->layouts)
	{
		//set ideal positioning depending on the layout-placement
		l->setIdealPosition();

		//calculate and set the bestplacement for the layout within th margin_spaces
		l->setBestLayoutPlacement();

		//update the parent-layouts margin_spaces to match with the newly placed layout
		//the parent_layout is in this case "this"
		l->updateParentMarginSpaces();

		         //use recursion to place the layouts inside of layout l
		//this way, we place every layout available inside of the whole "base"-layout
		l->placeLayouts();
	}
}

void Layout::setIdealPosition()
{

	sf::Vector2f pl_pos = this->parent_layout->getPosition();							//the parentlayouts position
	sf::Vector2f pl_size = this->parent_layout->getSize();								//the parentlayouts size

	//TOP
	if (this->layout_placement == LAYOUT_PLACEMENT::LP_TOP_CENTERED)
	{
		this->setPosition({ pl_pos.x + (pl_size.x / 2) - (this->rect.width / 2), pl_pos.y });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_TOP_LEFT)
	{
		this->setPosition(pl_pos);
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_TOP_RIGHT)
	{
		this->setPosition({ pl_pos.x + pl_size.x - this->rect.width , pl_pos.y });
	}
	//BOTTOM
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_BOTTOM_CENTERED)
	{
		this->setPosition({ pl_pos.x + (pl_size.x / 2) - (this->rect.width / 2) , pl_pos.y + pl_size.y - this->rect.height });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_BOTTOM_LEFT)
	{
		this->setPosition({ pl_pos.x , pl_pos.y + pl_size.y - this->rect.height });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_BOTTOM_RIGHT)
	{
		this->setPosition({ pl_pos.x + pl_size.x - this->rect.width , pl_pos.y + pl_size.y - this->rect.height });
	}
	//CENTERED
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_CENTERED_LEFT)
	{
		this->setPosition({ pl_pos.x , pl_pos.y + (pl_size.y / 2) - (this->rect.height / 2) });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_CENTERED_RIGHT)
	{
		this->setPosition({ pl_pos.x + pl_size.x - this->rect.width , pl_pos.y + (pl_size.y / 2) - (this->rect.height / 2) });
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_CENTERED)
	{
		this->setPosition({ pl_pos.x + (pl_size.x / 2) - (this->rect.width / 2) , pl_pos.y + (pl_size.y / 2) - (this->rect.height / 2) });
	}
	//CUSTOM and NONE
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_CUSTOM)
	{
		//continue with the given position
	}
	else if (this->layout_placement == LAYOUT_PLACEMENT::LP_NONE)
	{
		//no position ha been given
		//could be useful when implementing a drag and drop i.e. in hand/not placed
	}
}

void Layout::setBestLayoutPlacement()
{
	/*This function calculates the best possible placement for this layout.
	inside of the given margin_spaces. This layout has been given a position,
	in some cases its ideal position. This function then look at all of the available
	margin_spaces that it can be placed inside of and finds the closest possible position*/

	sf::FloatRect layout = getTotalRect();										//the whole layout, including padding and margins
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

void Layout::updateParentMarginSpaces()
{
	sf::FloatRect layout = this->getTotalRect();						//the whole layout, including padding and margins

	int index = 0;
	//index % 4 = 0 -> treat the margin as a top_margin. DECREASE HEIGHT
	//index % 4 = 1 -> treat the margin as a right_margin. INCREASE LEFT-POSITION AND DECREASE WIDTH
	//index % 4 = 2 -> treat the margin as a bottom_margin. INCREASE TOP-POSITION AND DECREASE HEIGHT
	//index % 4 = 3 -> treat the margin as a left_margin. DECREASE WIDTH

	//loop all of the parent_layout's margin_spaces
	for (auto& ms : this->parent_layout->getMarginSpaces())
	{
		
		sf::FloatRect instersect_area;
		if (ms.intersects(layout, instersect_area))
		{
			//positive diff: rect's left/right side is to the right of margin_space's left/right side.
			float right_diff = (layout.left + layout.width) - (ms.left + ms.width);
			float left_diff = layout.left - ms.left;

			//positive diff: rect's top/bottom side is below the margin_space's top/bottom side.
			float bottom_diff = (layout.top + layout.height) - (ms.top + ms.height);
			float top_diff = layout.top - ms.top;

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
						ms.left = layout.left + layout.width;
						ms.width = -right_diff;
						no_adjustment = 0;
					}
					else if (bottom_diff < 0)
					{
						ms.top = layout.top + layout.height;
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
					ms.left = layout.left + layout.width;
					ms.width = -right_diff;
					no_adjustment = 0;
				}
				//if not, choose another type of adjustment
				else
				{
					/*For right_margin, bottom adjustment is favored, then top and last left.*/

					if (bottom_diff < 0)
					{
						ms.top = layout.top + layout.height;
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
					ms.top = layout.top + layout.height;
					ms.height = -bottom_diff;
					no_adjustment = 0;
				}
				//if not, choose another type of adjustment
				else
				{
					/*For bottom_margin, right adjustment is favored, then left and last top.*/

					if (right_diff < 0)
					{
						ms.left = layout.left + layout.width;
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
						ms.top = layout.top + layout.height;
						ms.height = -bottom_diff;
						no_adjustment = 0;
					}
					else if (right_diff < 0)
					{
						ms.left = layout.left + layout.width;
						ms.width = -right_diff;
						no_adjustment = 0;
					}
				}
			}
		
			// Update the margin_rect's floatrect. 
			// The ms is in this case not updated by the operations above. This step is necessary
			this->parent_layout->setMarginSpaceRect(ms,index);
		}

		index++;
	}
}

void Layout::resetMarginSpaces()
{
	for (auto& ms : this->margin_spaces)
	{
		if (this->num_layouts == 0) return;

		ms = this->rect;
	}
	for (auto& l : this->layouts)
	{
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
	return sqrt(pow(d.x - s.x, 2) + pow(d.y - s.y, 2) * 1.0);
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

void Layout::setPosition(sf::Vector2f new_pos)
{
	sf::Vector2f move_dist = sf::Vector2f(new_pos.x - this->rect.left, new_pos.y - this->rect.top);		//the distance this layout will be moved

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

	//finally, use recursion to move all of the layouts inside of this one
	for (auto& child_layout : this->layouts)
	{
		child_layout->setPosition({ child_layout->getRect().left + move_dist.x, child_layout->getRect().top + move_dist.y });
	}
	
}

bool Layout::validSize(sf::Vector2f size)
{
	if (size.x < this->min_size.x || size.y < this->min_size.y)
	{
		return false;
	}
	return true;
}