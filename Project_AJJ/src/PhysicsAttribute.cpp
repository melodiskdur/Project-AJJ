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