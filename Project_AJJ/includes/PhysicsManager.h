#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "PhysicsAttribute.h"
#include "Hitbox.h"
#include "CollisionDetection.h"
#include <math.h>
#include "Gravity.h"
#include "AirFriction.h"

// "etc"

/* Represents a collision between two objects. The struct
* in itself is a directed edge between Collisios_Nodes,
* from m_node to m_adjacent. It contains information 
* of the collision in regards to m_node.object, such as
* which side was intersected and what the proposed 
* changes in position and velocity were to undo the
* collision.
*/
typedef struct _Collision_Edge
{
	struct _Collision_Node* m_node;
	struct _Collision_Node* m_adjacent;
	INTERSECTED_SIDE m_side;
	sf::Vector2f proposed_repos;
	sf::Vector2f proposed_revel;
} Collision_Edge;

/* Represents an object that has been involved in
* at least one collision in a given frame. The
* members 'm_updated_pos' and 'm_updated_vel' are
* "intermediate" new values for m_node_object. Note that
* they should not be assigned to m_node_object until the
* tree of collisions have been resolved. However, they
* are updated after collisions (edges) are resolved, as a
* way to signal that the object has been moved as a result.
* This is needed to recalibrate the remaining collisions (edges).
*/
typedef struct _Collision_Node
{
	Object* m_node_object;
	sf::Vector2f m_updated_pos;
	sf::Vector2f m_update_vel;
	sf::Vector2f m_o_size;
	std::vector<Collision_Edge> edges;

} Collision_Node;

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

	// Function to sort the collision data in descending order of number of concurrent collisions.
	// Contains an implementation of Insertion sort.
	void sortData();
};