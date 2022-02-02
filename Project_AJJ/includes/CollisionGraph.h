#pragma once
#include "PhysicsAttribute.h"
#include "Object.h"
#include <math.h>

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
	sf::Vector2f m_frame_pos;
	sf::Vector2f m_frame_vel;
	sf::Vector2f m_updated_pos;
	sf::Vector2f m_updated_vel;
	sf::Vector2f m_o_size;
	std::vector<Collision_Edge> edges;

} Collision_Node;

class CollisionGraph :
	public PhysicsAttribute
{
public:
	CollisionGraph();
	~CollisionGraph();
private:
};

