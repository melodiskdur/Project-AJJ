#pragma once
#include <SFML/Graphics.hpp>

/* Base class for all PhysicsAttribute-type classes. Those contain
*  collision detection, gravity, momentum, etc.
* 
*  PhysicsAttribute-type instances can be stored as pointers in 
*  Scene- and Object-instances. (WIP)
*/
class PhysicsAttribute
{
public:
	PhysicsAttribute(std::string attrib_name);
	virtual ~PhysicsAttribute();
	
	std::string getId();

protected:
	std::string attribute_name;
};

