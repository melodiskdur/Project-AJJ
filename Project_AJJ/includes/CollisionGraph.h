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
typedef struct _CollisionEdge
{
	struct _CollisionNode* m_node;
	struct _CollisionNode* m_adjacent;
	INTERSECTED_SIDE m_side;
	sf::Vector2f proposed_repos;
	sf::Vector2f proposed_revel;
} CollisionEdge;

/* Represents an object in the absolute vicinity of an object involved
* in a collision. That means any object whose distance to the collided
* object is less than the size of the collided object (the largest distance
  repositioning that we should expect). This is a way to keep the resolve
  of an object to cause another collision.
*/
typedef struct _CloseCall
{
	Object* m_closecall_object;
	struct _CollisionNode* m_node;
	sf::Vector2f m_distance;
	INTERSECTED_SIDE m_horizontal;
	INTERSECTED_SIDE m_vertical;
} CloseCall;

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
typedef struct _CollisionNode
{
	Object* m_node_object;
	sf::Vector2f m_frame_pos;
	sf::Vector2f m_frame_vel;
	sf::Vector2f m_updated_pos;
	sf::Vector2f m_updated_vel;
	sf::Vector2f m_o_size;
	std::vector<CollisionEdge> m_outgoing_edges;
	std::vector<CloseCall> m_close_calls;

} CollisionNode;

class CollisionGraph :
	public PhysicsAttribute
{
public:
	CollisionGraph();
	~CollisionGraph();
private:
};

