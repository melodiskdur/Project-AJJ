#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "PhysicsAttribute.h"
#include "Hitbox.h"
#include "CollisionDetection.h"
#include "CollisionGraph.h"
#include <math.h>
#include "Gravity.h"
#include "AirFriction.h"

// "etc" ...

class CollisionGraph;

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
	std::vector<INTERSECTED_SIDE> m_intersects;	    // Sides of the object hitbox where a collision has been detected.

	// Indices of best resolves.
	std::vector<int> indices;

	sf::Vector2f m_final_repos;
	sf::Vector2f m_final_revel;
	sf::Vector2f m_final_reacc;
} CollisionData;

enum COL_STACKTYPE {COL_HORIZONTAL, COL_VERTICAL};

typedef struct _StackedCollision
{
	Object* m_object;
	Object* m_collider1;
	Object* m_collider2;
	COL_STACKTYPE m_stacktype;
} StackedCollision;

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

	// DEBUGGING.
	CollisionGraph* col_graph;
	// END DEBUGGING.
private:
	std::vector<PhysicsAttribute*> attributes;
	std::vector<Object*>* scene_objects = nullptr;
	std::vector<CollisionData> data;
	std::vector<StackedCollision> stacked_collisions;

	void storeObjectData(ObjectData odata);
	int indexOf(Object* o);

	// Self-explanatory. Object positions are explicitly set in this function.
	void setAverageHitboxRes(CollisionData& data);
	// Returns all the (sf::FloatRect) hitboxes for all the colliding objects of a given object.
	std::vector<sf::FloatRect> getHitboxes(CollisionData& data);
	// Function to sort the collision data in descending order of number of concurrent collisions.
	// Contains an implementation of Insertion sort.
	void sortData();
};