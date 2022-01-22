#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

/* Simple struct used to store changes made to an object's position,
* velocity or acceleration that's been done in any PhysicsAttribute
* subtype. The idea is then to update them at the end of the 'Physics-pipeline'.
*/
typedef struct _ObjectData
{
	Object* m_object;
	Object* m_colliding_object;
	sf::Vector2f m_wp;
	sf::Vector2f m_vel;
	sf::Vector2f m_acc;
	sf::FloatRect m_colliding_hitbox;
} ObjectData;

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

