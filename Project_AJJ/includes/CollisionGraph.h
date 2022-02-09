#pragma once
#include "PhysicsAttribute.h"
#include "CollisionDetection.h"
#include "Hitbox.h"
#include "Object.h"
#include <math.h>

#define REPOS_INFINITY 200000.f

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
	sf::Vector2f m_proposed_resolve = { REPOS_INFINITY, REPOS_INFINITY };
	std::vector<_CollisionEdge*> m_bonus_resolves;
	bool m_is_relaxed = false;
	bool m_is_visited = false;
	int m_storage_index;
} CollisionEdge;

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
	sf::Vector2f m_updated_pos;
	std::vector<CollisionEdge*> m_primary;
	std::vector<CollisionEdge*> m_relaxed;
	std::vector<CollisionEdge*> m_triggered;
	std::vector<CollisionEdge*> m_unresolved;
	int m_storage_index;
} CollisionNode;

/*
* INFO HERE
*/
class CollisionGraph :
	public PhysicsAttribute
{
public:
	CollisionGraph();
	~CollisionGraph();

	// Reads data from a vector of collision tuples and creates
	// nodes and edges accordingly. Note that resolves are calculating as well.
	void storeCollisions(std::vector<ObjectTuple> collision_tuples);

	// Looks for the CloseCallHolder.m_object node and stores the close calls
	// as relaxed edges.
	void storeCloseCalls(std::vector<CloseCallHolder>& close_calls);

	// Creates and stores a CollisionNode for odata.m_object. Returns
	// the index of the node in vector node_storage.
	int createNode(ObjectData& odata);
	int createNode(Object* object);

	// Creates and stores a CollisionEdge for odata.m_object.
	void createEdge(ObjectData& odata);
	void createEdge(ObjectData& odata, CollisionNode* node, CollisionNode* adjacent);
	// Adds a collision close call to 'node' by creating a CollisionEdge and setting it as relaxed. 
	void createCloseCallEdge(CollisionNode* node, CollisionNode* adjacent);

private:
	// Node storage vector + pointer vectors.
	std::vector<CollisionNode> node_storage;
	std::vector<CollisionNode*> primary_collisions;
	std::vector<CollisionNode*> triggered_collisions;
	std::vector<CollisionNode*> relaxed_collisions;
	std::vector<CollisionNode*> unresolved_collisions;

	// Edge storage vector + pointer vectors.
	std::vector<CollisionEdge> edge_storage;
	std::vector<CollisionNode*> active_edges;

	// Used internally to check if an object already has a CollisonNode.
	// Returns -1 if the node doesn't exist.
	int indexOf(Object* o);

	// Used internally to sort the primary edges in number
	// of resolves that are created (one edge.proposed
	// could for example solve 2 other collisions).
	void sortEdges(CollisionNode* c);

	// Used internally to reset all the vectors.
	void clearVectors();
};

