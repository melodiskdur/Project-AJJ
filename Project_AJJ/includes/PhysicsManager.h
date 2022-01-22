#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "PhysicsAttribute.h"
#include "Hitbox.h"
#include "CollisionDetection.h"
#include <math.h>

//#include "Gravity.h"
// "etc"

/* Datatype to store all the changes to an object's parameters that's
* been calculated in the different PhysicsAttributes. 
*/
typedef struct _CollisionData
{
	Object* m_object;
	std::vector<sf::Vector2f> m_repositions;		// New positions.
	std::vector<sf::Vector2f> m_revelocities;		// New velocities.
	std::vector<sf::Vector2f> m_reacc;				// New accelerations.
	std::vector<Object*> m_colliding_objects;		// All the colliding objects.
	// Note that all the vectors above should have the same size.
	std::vector<sf::FloatRect> m_hitboxes;			// Hitboxes of m_colliding_objects.

	sf::Vector2f m_final_repos;
	sf::Vector2f m_final_revel;
	sf::Vector2f m_final_reacc;
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

	void storeObjectData(ObjectData odata);
	int indexOf(Object* o);

	// Self-explanatory. Object positions are explicitly set in this function.
	void setAverageHitboxRes(CollisionData& data);
	// Returns all the (sf::FloatRect) hitboxes for all the colliding objects of a given object.
	std::vector<sf::FloatRect> getHitboxes(CollisionData& data);
};