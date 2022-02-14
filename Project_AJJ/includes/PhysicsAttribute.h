#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

enum INTERSECTED_SIDE { ODATA_TOP, ODATA_RIGHT, ODATA_BOTTOM, ODATA_LEFT, ODATA_NONE = -1 };

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
	INTERSECTED_SIDE m_intersect = ODATA_NONE;
} ObjectData;

/* Base class for all PhysicsAttribute-type classes. Those contain
*  collision detection, gravity, momentum, etc.
* 
*  PhysicsAttribute-type instances can be stored as pointers in 
*  Scene-instances. (WIP)
*/
class PhysicsAttribute
{
public:
	PhysicsAttribute(std::string attrib_name);
	virtual ~PhysicsAttribute();
	
	// Getters
	std::string getId();

	// Setters
	
	//Others
	
	// Returns the opposite INTERSECTED_SIDE
	static INTERSECTED_SIDE oppositeSide(INTERSECTED_SIDE& side);

protected:
	std::string attribute_name;
};

