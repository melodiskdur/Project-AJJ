#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsAttribute.h"
#include "CollisionDetection.h"
#include "Hitbox.h"
#include "Object.h"
#include <math.h>

#define REPOS_INFINITY 200000000.f

typedef struct _ObjectTuple ObjectTuple;
typedef struct _CloseCallHolder CloseCallHolder;

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
	int m_i_node;
	int m_i_adjacent;
	INTERSECTED_SIDE m_side;
	sf::Vector2f m_proposed_resolve = { REPOS_INFINITY, REPOS_INFINITY };
	std::vector<int> i_m_bonus_resolves;
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
	std::vector<int> i_m_primary;
	std::vector<int> i_m_relaxed;
	std::vector<int> i_m_triggered;
	std::vector<int> i_m_unresolved;
	int m_storage_index;
	// DEBUGGING.
	int m_num_static = 0;
	// END DEBUGGING.
	bool m_horizontal_collision = false;
	bool m_vertical_collision = false;
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

	// Main function of the graph. Attempts to relax all the edges
	// of the graph and then store the resulting resolves for each CollisionNode.
	void resolveTree();

	// Helper-function to resolveTree().
	void resolveCollision(CollisionEdge* e, CollisionNode* c);

	// (WIP) Applies (onto object.worldPosition) all the calculated resolves from resolveTree().
	void applyTreeResolution();

	// Reads data from a vector of collision tuples and creates
	// nodes and edges accordingly. Note that resolves are calculating as well.
	void storeCollisions(std::vector<ObjectTuple> collision_tuples);

	// Looks for the CloseCallHolder.m_object node and stores the close calls
	// as relaxed edges.
	void storeCloseCalls(std::vector<CloseCallHolder>& close_calls);

	// Creates and stores a CollisionNode for an Object instance. Returns
	// the index of the node in vector node_storage.
	int createNode(Object* object);

	// Creates and stores a CollisionEdge for odata.m_object.
	void createEdge(ObjectData& odata);
	// Adds a collision close call to 'node' by creating a CollisionEdge and setting it as relaxed. 
	void createCloseCallEdge(CollisionNode* node, CollisionNode* adjacent);

private:
	// Node storage vector + index vectors.
	std::vector<CollisionNode> node_storage;
	std::vector<int> i_primary_collisions;
	std::vector<int> i_triggered_collisions;
	std::vector<int> i_relaxed_collisions;
	std::vector<int> i_unresolved_collisions;

	// Edge storage vector + index vectors.
	std::vector<CollisionEdge> edge_storage;
	std::vector<int> i_active_edges;
	std::vector<int> i_primary_edges;

	// Applies e.proposed_resolve to e. Also marks the e.bonus_resolves-Edges as relaxed.
	void updatePosition(CollisionNode* c, CollisionEdge* e);

	// When a proposed resolve has been applied to a Node, this function checks the remaining
	// Edges in c.primary to see if their proposed resolves need to be adjusted relative to the
	// c.updated_pos.
	void recalibrateResolves(CollisionNode* c);

	// Used within resolveCollision().
	void handleTriggeredCollisions(CollisionNode* c, CollisionEdge* e, const std::vector<int>& i_triggered);

	// Executes whenever a resolve triggers a (permittable) relaxed collision.
	void collisionTriggered(CollisionEdge* r, CollisionEdge* e);

	// Executes whenever a proposed resolve triggers a collision with a static object.
	void markAsUnresolved(CollisionEdge* unres);

	// Checks a given parameter vector for an index of an Edge. That has node_storage[i_c] as Node
	// and node_storage[i_adj] as adjacent.
	int findEdgeIndex(int i_c, int i_adj, std::vector<int>& edge_vector);

	CollisionEdge* invertedEdge(CollisionEdge* e);

	// Checks if two objects are colliding into each other.
	bool collisionTwoSided(CollisionEdge* e);

	// Used internally to check if an object already has a CollisonNode.
	// Returns -1 if the node doesn't exist.
	int indexOf(Object* o);
	// Sorts the primary edges in number
	// of resolves that are created (one edge.proposed
	// could for example solve 2 other collisions).
	void sortEdges(CollisionNode* c);
	// Runs sortEdges (see above) on each object of the primary_collisions vector.
	// It runs correctly under the assumption that primary_collisions has been sorted beforehand.
	void sortAllPrimaryEdges();
	// Sorts the primary CollisionNode in
	// descending order of collisions it's involved in.
	void sortPrimaryCollisions();
	// Same as above but in order of STATIC collisions it's involved in.
	void sortPrimaryByStatic();
	// Resets all the vectors.
	void clearVectors();
	// Checks if e->proposed_resolve also resolves the edge e_tocheck.
	bool resolves(CollisionEdge* e, CollisionEdge* e_tocheck);
	// Checks for intersection between an active and a relaxed Edge.
	bool intersects(CollisionEdge* e, CollisionEdge* r);

};

