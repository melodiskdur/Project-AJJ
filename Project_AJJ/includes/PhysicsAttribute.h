#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

enum INTERSECTED_SIDE { ODATA_TOP, ODATA_RIGHT, ODATA_BOTTOM, ODATA_LEFT, ODATA_NONE = -1 };

/* Base class for all PhysicsAttribute-type classes. Those contain
*  collision detection, gravity, momentum, etc.
* 
*  PhysicsAttribute-type instances can be stored as pointers in 
*  Scene-instances. (WIP)
*/
class PhysicsAttribute
{
public:
	PhysicsAttribute();
	PhysicsAttribute(std::string attrib_name);
	virtual ~PhysicsAttribute();
	
	// Getters
	std::string getId();

	// Setters
	
	//Others
	
	// Returns the opposite INTERSECTED_SIDE
	static INTERSECTED_SIDE oppositeSide(INTERSECTED_SIDE& side);
	sf::String intersectString(INTERSECTED_SIDE& side);

	static unsigned int instanceCount() { return instance_counter; };
protected:
	std::string attribute_name;

private:
	static unsigned int instance_counter;
};

