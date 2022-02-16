#include "PhysicsAttribute.h"

PhysicsAttribute::PhysicsAttribute(std::string attrib_name)
{
	attribute_name = attrib_name;
}

PhysicsAttribute::~PhysicsAttribute()
{

}

//Getters
std::string PhysicsAttribute::getId()
{
	return this->attribute_name;
}



//Setters

//Others
INTERSECTED_SIDE PhysicsAttribute::oppositeSide(INTERSECTED_SIDE& side)
{
	switch (side)
	{
	case INTERSECTED_SIDE::ODATA_TOP: return INTERSECTED_SIDE::ODATA_BOTTOM;
	case INTERSECTED_SIDE::ODATA_RIGHT: return INTERSECTED_SIDE::ODATA_LEFT;
	case INTERSECTED_SIDE::ODATA_BOTTOM: return INTERSECTED_SIDE::ODATA_TOP;
	case INTERSECTED_SIDE::ODATA_LEFT: return INTERSECTED_SIDE::ODATA_RIGHT;
	default: return INTERSECTED_SIDE::ODATA_NONE;
	}
}

sf::String PhysicsAttribute::intersectString(INTERSECTED_SIDE& side)
{
	switch (side)
	{
	case INTERSECTED_SIDE::ODATA_TOP: return sf::String("TOP");
	case INTERSECTED_SIDE::ODATA_RIGHT: return sf::String("RIGHT");
	case INTERSECTED_SIDE::ODATA_BOTTOM: return sf::String("BOTTOM");
	case INTERSECTED_SIDE::ODATA_LEFT: return sf::String("LEFT");
	default: return sf::String("NONE");
	}
}
