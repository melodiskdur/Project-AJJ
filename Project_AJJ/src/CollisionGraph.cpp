#include "CollisionGraph.h"

CollisionGraph::CollisionGraph()
	: PhysicsAttribute("CollisionGraph")
{

}

CollisionGraph::~CollisionGraph()
{

}

void CollisionGraph::storeCollisions(std::vector<ObjectTuple> collision_tuples)
{
	// Clear vectors before storing. Refactored for readability.
	this->clearVectors();
	for (auto& tuple: collision_tuples)
	{
		// Hitbox resolve calculations before creating and storing.
		std::vector<ObjectData> v_odata = Hitbox::separateHitboxes(tuple.obj_i, tuple.obj_j);
		// Creating and storing.
		for (auto& odata : v_odata)
		{
			int i_odata = indexOf(odata.m_object);
			int i_odata_collided = indexOf(odata.m_colliding_object);
			// Create CollisionNode for m_object if it doesn't already exist.
			if (i_odata == -1) i_odata = this->createNode(odata);
			//Create CollisionNode for m_colliding_object as well if necessary.
			if (i_odata_collided == -1) i_odata_collided = this->createNode(odata.m_colliding_object);
			// Create and store CollisionEdge.
			this->createEdge(odata, &this->node_storage[i_odata], &this->node_storage[i_odata_collided]);
		}
	}
}

void CollisionGraph::storeCloseCalls(std::vector<CloseCallHolder>& close_calls)
{
	for (auto& holder : close_calls)
	{
		int i_holder_object = indexOf(holder.m_object);
		// Create CollisionNode for m_object if it doesn't exist.
		if (i_holder_object == -1) i_holder_object = this->createNode(holder.m_object);
		// Store close call-edges.
		for (auto& close_call : holder.m_close_calls)
		{
			int i_closecall_object = indexOf(close_call);
			// Create CollisionNode for close_call if it doesn't exist.
			if (i_closecall_object == -1) i_closecall_object = this->createNode(close_call);
			// Create close call edge.
			this->createCloseCallEdge(&this->node_storage[i_holder_object], &this->node_storage[i_closecall_object]);
		}
	}
}

int CollisionGraph::createNode(ObjectData& odata)
{
	CollisionNode new_node;
	new_node.m_node_object = odata.m_object;
	new_node.m_frame_pos = odata.m_object->getWorldPosition();
	new_node.m_updated_pos = new_node.m_frame_pos;
	new_node.m_storage_index = this->node_storage.size();

	this->node_storage.push_back(new_node);

	// Store pointer in G.primaryCollisions.
	CollisionNode* node_ptr = &this->node_storage[new_node.m_storage_index];
	this->primary_collisions.push_back(node_ptr);

	return new_node.m_storage_index;
}

int CollisionGraph::createNode(Object* object)
{
	CollisionNode new_node;
	new_node.m_node_object = object;
	new_node.m_frame_pos = object->getWorldPosition();
	new_node.m_updated_pos = new_node.m_frame_pos;
	new_node.m_storage_index = this->node_storage.size();

	// Store pointer in G.primaryCollisions.
	CollisionNode* node_ptr = &this->node_storage[new_node.m_storage_index];
	this->primary_collisions.push_back(node_ptr);

	this->node_storage.push_back(new_node);
	return new_node.m_storage_index;
}

void CollisionGraph::createEdge(ObjectData& odata)
{
	int i_node = this->indexOf(odata.m_object);
	int i_colliding = this->indexOf(odata.m_colliding_object);
	// Exit function if at least one of the CollisionNodes is NOT found.
	if (i_node == -1 || i_colliding == -1) return;
	// Else call the other createEdge-function where the functionality is implemented.
	this->createEdge(odata, &this->node_storage[i_node], &this->node_storage[i_colliding]);
}

void CollisionGraph::createEdge(ObjectData& odata, CollisionNode* node, CollisionNode* adjacent)
{
	CollisionEdge new_edge;
	new_edge.m_node = node;
	new_edge.m_adjacent = adjacent;
	new_edge.m_proposed_resolve = odata.m_wp;
	new_edge.m_side = odata.m_intersect;
	new_edge.m_storage_index = this->edge_storage.size();

	this->edge_storage.push_back(new_edge);

	// Store pointer to the CollisionEdge in m_primary.
	CollisionEdge* edge_ptr = &this->edge_storage[new_edge.m_storage_index];
	node->m_primary.push_back(edge_ptr);
}

void CollisionGraph::createCloseCallEdge(CollisionNode* node, CollisionNode* adjacent)
{
	CollisionEdge new_edge;
	new_edge.m_node = node;
	new_edge.m_adjacent = adjacent;
	// Set the edge as relaxed.
	new_edge.m_is_relaxed = true;

	// Store pointer to edge in m_relaxed.
	CollisionEdge* edge_ptr = &this->edge_storage[new_edge.m_storage_index];
	node->m_relaxed.push_back(edge_ptr);
}

int CollisionGraph::indexOf(Object* o)
{
	for (int i = 0; i < this->node_storage.size(); i++)
		// Return index if equivalent object is found.
		if (this->node_storage[i].m_node_object == o) return i;
	// Return -1 if no corresponding CollisionNode is found.
	return -1;
}

void CollisionGraph::sortEdges(CollisionNode* c)
{
}

void CollisionGraph::clearVectors()
{
	this->node_storage.clear();
	this->primary_collisions.clear();
	this->triggered_collisions.clear();
	this->relaxed_collisions.clear();
	this->unresolved_collisions.clear();
	this->edge_storage.clear();
	this->active_edges.clear();
}
