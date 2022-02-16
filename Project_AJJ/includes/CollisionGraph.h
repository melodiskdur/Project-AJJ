#pragma once
#include <SFML/Graphics.hpp>
#include "PhysicsAttribute.h"
#include "CollisionDetection.h"
#include "Hitbox.h"
#include "Object.h"
#include <math.h>

#define REPOS_INFINITY 200000000.f

enum class EDGE_STATUS { EDGE_ACTIVE, EDGE_RELAXED, EDGE_TRIGGERED, EDGE_UNRESOLVED };

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
	std::vector<int> i_m_all;
	std::vector<int> i_m_primary;
	std::vector<int> i_m_relaxed;
	std::vector<int> i_m_triggered;
	std::vector<int> i_m_unresolved;
	int m_storage_index;
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
	void createInvertedEdge(CollisionEdge* e);

	// Adds a collision close call to 'node' by creating a CollisionEdge and setting it as relaxed. 
	void createCloseCallEdge(CollisionNode* node, CollisionNode* adjacent);

	// Debugging, treedrawer.
	std::vector<sf::VertexArray> getTree();
private:
	// Node storage vector + index vectors.
	std::vector<CollisionNode> node_storage;
	std::vector<int> i_active_nodes;

	// Edge storage vector + index vectors.
	std::vector<CollisionEdge> edge_storage;
	std::vector<int> i_active_edges;
	std::vector<int> i_primary_edges;
	std::vector<int> i_triggered_edges;
	std::vector<int> i_relaxed_edges;
	std::vector<int> i_unresolved_edges;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*       Updating / manipulating Nodes & Edges            *
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*                   Comparisons                          *
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

	// Checks if two objects are colliding into each other.
	bool collisionTwoSided(CollisionEdge* e);
	// Check if an Edge e with a STATIC adjacent object triggers an Edge r with a STATIC object.
	bool staticTriggersStatic(CollisionEdge* e, CollisionEdge* r);
	// Checks if e->proposed_resolve also resolves the edge e_tocheck.
	bool resolves(CollisionEdge* e, CollisionEdge* e_tocheck);
	// Checks for intersection between an active and a relaxed Edge.
	bool intersects(CollisionEdge* e, CollisionEdge* r);
	// Checks for intersection between two Collision Nodes.
	bool intersects(CollisionNode* c, CollisionNode* c_adj);

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*                Finding Nodes and Edges.                *
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

	// Checks a given parameter vector for an index of an Edge. That has node_storage[i_c] as Node
	// and node_storage[i_adj] as adjacent.
	int findEdgeIndex(int i_c, int i_adj, std::vector<int>& edge_vector);
	int findNodeIndexIn(const int node_index, std::vector<int>& i_node_vector);

	CollisionEdge* invertedEdge(CollisionEdge* e);

	// Pointer (to node/edge_storage instances) returners, for cleaner code.
	CollisionNode* findNode(Object* o);
	CollisionNode* findNode(int node_index);
	CollisionEdge* findEdge(int edge_index);
	std::vector<CollisionNode*> findOutgoingAdjacents(int node_index);
	std::vector<CollisionNode*> findOutgoingAdjacents(CollisionNode* c);
	std::vector<CollisionNode*> findIncomingAdjacents(int node_index);
	std::vector<CollisionNode*> findIncomingAdjacents(CollisionNode* c);
	std::vector<CollisionEdge*> findIncomingEdges(int node_index);
	std::vector<CollisionEdge*> findIncomingEdges(CollisionNode* c);
	std::vector<CollisionEdge*> findOutgoingEdges(int node_index);
	std::vector<CollisionEdge*> findOutgoingEdges(CollisionNode* c);
	std::vector<CollisionEdge*> findOutgoingEdgesByStatus(CollisionNode* c, EDGE_STATUS status);
	std::vector<CollisionEdge*> findEdgesByVector(const std::vector<int>& edge_indices);
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	*                Sorting-methods.                        *
	* * * * * * * * * * * * * * * * * * * * * * * * * * * * **/

	// Methods to switch index-integers between vectors.
	void moveEdgeIndex(CollisionEdge* e, std::vector<int>& from, std::vector<int>& to);
	void moveNodeIndex(CollisionNode* c, std::vector<int>& from, std::vector<int>& to);

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
	// Sorts g.primary_edges by collision type. Double collisions are prioritized.
	void sortPrimaryEdgesByType();
	// Resets all the vectors.
	void clearVectors();

	// DEBUGGING.
	CollisionNode* node1337 = nullptr;
	// END DEBUGGING.

};

