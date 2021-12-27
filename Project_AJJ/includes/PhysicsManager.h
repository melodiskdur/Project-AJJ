#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "PhysicsAttribute.h"
#include "Hitbox.h"
#include "CollisionDetection.h"
//#include "Gravity.h"

typedef struct _CollisionData
{
	Object* m_object;
	std::vector<sf::Vector2f> m_repositions;		// New positions.
	std::vector<sf::Vector2f> m_revelocities;		// New velocities.
	std::vector<sf::Vector2f> m_reacc;				// New accelerations.
} CollisionData;

/* PhysicsManager
* Handles all physics-related things.
*/
class PhysicsManager
{
public:
	PhysicsManager(std::vector<Object*>& objects);
	~PhysicsManager();

	void addAttribute(PhysicsAttribute* attribute);
	void basicCollisionHandler(sf::FloatRect view_rect);
	PhysicsAttribute* searchAttribute(std::string attribute_name);
private:
	std::vector<PhysicsAttribute*> attributes;
	std::vector<Object*>* scene_objects = nullptr;
	std::vector<CollisionData> data;

	void createCollisionData(Object* o);
	void storeVelocity(sf::Vector2f);
	void storePosition(sf::Vector2f);
	void storeAcc(sf::Vector2f);
};