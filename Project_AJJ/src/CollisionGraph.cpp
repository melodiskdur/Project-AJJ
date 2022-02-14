#include "CollisionGraph.h"

CollisionGraph::CollisionGraph()
	: PhysicsAttribute("CollisionGraph")
{

}

CollisionGraph::~CollisionGraph()
{

}

void CollisionGraph::resolveTree()
{
	
}

void CollisionGraph::resolveCollision(CollisionEdge* e, CollisionNode* c)
{
	
}

void CollisionGraph::applyTreeResolution()
{
	for (CollisionNode& node : this->node_storage)
	{
		node.m_node_object->setWorldPosition(node.m_updated_pos);

		// DEBUGGING
		// THIS SHOULD PERHAPS NOT BE HERE.
		sf::Vector2f o_velocity = node.m_node_object->getVelocity();
		if (node.m_horizontal_collision) o_velocity.x = 0.0f;
		if (node.m_vertical_collision) o_velocity.y = 0.0f;
		node.m_node_object->setVelocity(o_velocity);
		// END DEBUGGING.
	}
}

void CollisionGraph::storeCollisions(std::vector<ObjectTuple> collision_tuples)
{
	this->clearVectors();
	for (ObjectTuple tuple : collision_tuples)
	{
		// For each object in the tuple, check if there's a CollisionNode. If not, create it.
		int index_i = this->indexOf(tuple.obj_i);
		if (index_i == -1) index_i = this->createNode(tuple.obj_i);

		int index_j = this->indexOf(tuple.obj_j);
		if (index_j == -1) index_j = this->createNode(tuple.obj_j);

		// Calculate Hitbox resolves and create the corresponding CollisionEdges.
		std::vector<ObjectData> v_odata = Hitbox::separateHitboxes(tuple.obj_i, tuple.obj_j);
		for (ObjectData odata : v_odata)
		{
			this->createEdge(odata);
		}
	}
}

void CollisionGraph::storeCloseCalls(std::vector<CloseCallHolder>& close_calls)
{
	for (CloseCallHolder holder : close_calls)
	{
		int i_holder = this->indexOf(holder.m_object);
		if (i_holder == -1) i_holder = this->createNode(holder.m_object);
		// Create Nodes, as well as Edges from holder.m_object to each close call object.
		for (Object* cc : holder.m_close_calls)
		{
			int i_cc = this->indexOf(cc);
			if (i_cc == -1) i_cc = this->createNode(cc);
			this->createCloseCallEdge(&this->node_storage[i_holder], &this->node_storage[i_cc]);
		}
	}
}

int CollisionGraph::createNode(Object* object)
{
	// Double check that the Node doesn't already exist.
	if (this->indexOf(object) != -1) return this->indexOf(object);

	// Set up and store Node.
	CollisionNode new_node;
	new_node.m_node_object = object;
	new_node.m_frame_pos = object->getWorldPosition();
	new_node.m_updated_pos = new_node.m_frame_pos;
	new_node.m_storage_index = this->node_storage.size();

	// Node storage.
	this->node_storage.push_back(new_node);
	// Node index storage for later use.
	this->i_primary_collisions.push_back(new_node.m_storage_index);
	return new_node.m_storage_index;
}

void CollisionGraph::createEdge(ObjectData& odata)
{
	int i_object = indexOf(odata.m_object);
	int i_adjacent = indexOf(odata.m_colliding_object);
	// Exit if at least one Node is missing.
	if (i_object == -1 || i_adjacent == -1) return;
	// Setting up the new Edge.
	CollisionEdge new_edge;
	new_edge.m_i_node = i_object;
	new_edge.m_i_adjacent = i_adjacent;
	new_edge.m_proposed_resolve = odata.m_wp;
	new_edge.m_storage_index = this->edge_storage.size();
	new_edge.m_side = odata.m_intersect;

	// Increment number of STATIC collisions for i_object Node.
	if ((&this->node_storage[i_adjacent])->m_node_object->getBehaviorType() == OBJECT_BEHAVIOR::STATIC)
		(&this->node_storage[i_object])->m_num_static++;

	// Store Edge and add index to the storage to object's Node.primary.
	this->edge_storage.push_back(new_edge);
	this->i_active_edges.push_back(new_edge.m_storage_index);
	this->node_storage[i_object].i_m_primary.push_back(new_edge.m_storage_index);
}

void CollisionGraph::createCloseCallEdge(CollisionNode* node, CollisionNode* adjacent)
{
	// Setting up the new Edge.
	CollisionEdge new_edge;
	new_edge.m_i_node = node->m_storage_index;
	new_edge.m_i_adjacent = adjacent->m_storage_index;
	new_edge.m_proposed_resolve = node->m_updated_pos;
	new_edge.m_storage_index = this->edge_storage.size();
	new_edge.m_side = INTERSECTED_SIDE::ODATA_NONE;
	// Set edge as relaxed.
	new_edge.m_is_relaxed = true;

	// Store Edge and add index to the storage to object's Node.relaxed.
	this->edge_storage.push_back(new_edge);
	this->i_active_edges.push_back(new_edge.m_storage_index);
	this->node_storage[new_edge.m_i_node].i_m_relaxed.push_back(new_edge.m_storage_index);
}

void CollisionGraph::updatePosition(CollisionNode* c, CollisionEdge* e)
{
	// To avoid position-stacking, only add the difference between e.proposed_resolve and c.updated_pos to c.updated_pos.
	sf::Vector2f pos_diff = e->m_proposed_resolve - c->m_updated_pos;
	c->m_updated_pos += pos_diff;
	e->m_is_visited = true;
	e->m_is_relaxed = true;
	// Pop e.index from c.i_primary and move to relaxed.
	int i_primary_index = this->findEdgeIndex(e->m_i_node, e->m_i_adjacent, c->i_m_primary);
	if (i_primary_index != -1)
	{
		c->i_m_primary.erase(c->i_m_primary.begin() + i_primary_index);
		c->i_m_relaxed.push_back(e->m_storage_index);
	}
	// Mark e.bonus_resolves as relaxed. Pop the bonus edges from c.primary and move to c.relaxed.
	for (int edge_index : e->i_m_bonus_resolves)
	{
		CollisionEdge* bonus_edge = &this->edge_storage[edge_index];
		bonus_edge->m_is_relaxed = true;

		// Locate the bonus_edge index i c.primary and move it to c.relaxed
		i_primary_index = this->findEdgeIndex(bonus_edge->m_i_node, bonus_edge->m_i_adjacent, c->i_m_primary);
		if (i_primary_index != -1)
		{
			c->i_m_primary.erase(c->i_m_primary.begin() + i_primary_index);
			c->i_m_relaxed.push_back(edge_index);
		}
	}
	// Set intersected side.
	switch (e->m_side)
	{
		case ODATA_TOP: c->m_vertical_collision = true; break;
		case ODATA_LEFT: c->m_horizontal_collision = true; break;
		case ODATA_BOTTOM: c->m_vertical_collision = true; break;
		case ODATA_RIGHT: c->m_horizontal_collision = true; break;
	}
	// Finally, do a recalibration check for the rest of the Edges in m.primary.
	this->recalibrateResolves(c);
}

void CollisionGraph::recalibrateResolves(CollisionNode* c)
{
	for (int edge_index : c->i_m_primary)
	{
		CollisionEdge* e = &(this->edge_storage[edge_index]);
		CollisionNode* adj = &(this->node_storage[e->m_i_adjacent]);
		CollisionEdge* e_inv = this->invertedEdge(e);
		// If there is no inverted Edge (e.g if the adjacent Node has a STATIC object), skip this iteration.
		if (e_inv == nullptr) continue;
		// Wrap the necessary information into an ObjectData-instance.
		ObjectData temp_data;
		temp_data.m_object = c->m_node_object;
		temp_data.m_colliding_object = adj->m_node_object;
		temp_data.m_intersect = e->m_side;
		temp_data.m_wp = e->m_proposed_resolve;
		// Call external Hitbox-function to calculate the updated proposed resolves.
		// Then store the new resolves into it's respective edge.
		std::vector<sf::Vector2f> recab_proposed_resolves = Hitbox::recalibrate(temp_data, c->m_updated_pos, e_inv->m_side, adj->m_updated_pos);
		e->m_proposed_resolve = recab_proposed_resolves[0];
		e_inv->m_proposed_resolve = recab_proposed_resolves[1];
	}
}

void CollisionGraph::handleTriggeredCollisions(CollisionNode* c, CollisionEdge* e, const std::vector<int>& i_triggered)
{
	for (int trig_index : i_triggered)
	{
		// Safety check, to avoid out-of-range error. If used correctly, this condition should never be satisfied.
		if (trig_index >= this->edge_storage.size()) continue;
		CollisionEdge* r_trig = &this->edge_storage[trig_index];
		this->collisionTriggered(r_trig, e);
	}
}

void CollisionGraph::collisionTriggered(CollisionEdge* r, CollisionEdge* e)
{
	// Make sure r and e are pointing from the same node.
	if (r->m_i_node != e->m_i_node) return;
	r->m_is_visited = true;
	// We prioritize e.m_node in the sense that e.m_node is pushing on r.adjacent and not the other way around.
	// Thus, find the flipped edge of r ( opposite direction (adjacent, node) of r ).
	int i_flipped_r = this->findEdgeIndex(r->m_i_adjacent, r->m_i_node, this->i_active_edges);
	if (i_flipped_r == -1) return;

	// Calculate proposed resolve and set up r_flipped.
	// Positions and sizes.
	sf::Vector2f c_pos = this->node_storage[r->m_i_node].m_updated_pos;
	sf::Vector2f c_size = this->node_storage[r->m_i_node].m_node_object->getSize();
	sf::Vector2f adj_pos = this->node_storage[r->m_i_adjacent].m_updated_pos;
	sf::Vector2f adj_size = this->node_storage[r->m_i_adjacent].m_node_object->getSize();

	// Edge setup.
	CollisionEdge* r_flipped = &this->edge_storage[i_flipped_r];
	r_flipped->m_is_relaxed = false;
	r_flipped->m_side = this->oppositeSide(e->m_side);
	
	// Node setup.
	// Move r_flipped from adjacent.i_relaxed to adjacent.i_triggered.
	CollisionNode* r_adjacent = &this->node_storage[r_flipped->m_i_node];
	int edge_relaxed_index = this->findEdgeIndex(r_flipped->m_i_node, r_flipped->m_i_adjacent, r_adjacent->i_m_relaxed);
	if (edge_relaxed_index == -1) return;
	r_adjacent->i_m_relaxed.erase(r_adjacent->i_m_relaxed.begin() + edge_relaxed_index);
	r_adjacent->i_m_primary.push_back(r_flipped->m_storage_index);

	// Store index to node in graph.i_triggered_collisions.
	this->i_triggered_collisions.push_back(r_adjacent->m_storage_index);
}

void CollisionGraph::markAsUnresolved(CollisionEdge* unres)
{
	unres->m_is_visited = true;
	CollisionNode* node = &this->node_storage[unres->m_i_node];
	int edge_node_index = this->findEdgeIndex(unres->m_i_node, unres->m_i_adjacent, node->i_m_primary);
	if (edge_node_index == -1) return;
	// Remove from node.primary and put in node.unresolved.
	node->i_m_primary.erase(node->i_m_primary.begin() + edge_node_index);
	node->i_m_unresolved.push_back(unres->m_storage_index);
}

int CollisionGraph::findEdgeIndex(int i_c, int i_adj, std::vector<int>& edge_vector)
{
	for (int i = 0; i < edge_vector.size(); i++)
	{
		int edge_index = edge_vector[i];
		// Safety check, to avoid out-of-range error. If used correctly, this condition should never be satisfied.
		if (edge_index >= this->edge_storage.size()) continue;

		CollisionEdge* edge = &this->edge_storage[edge_index];
		if (edge->m_i_node == i_c && edge->m_i_adjacent == i_adj) return i;
	}
	return -1;
}

CollisionEdge* CollisionGraph::invertedEdge(CollisionEdge* e)
{
	int edge_index = this->findEdgeIndex(e->m_i_adjacent, e->m_i_node, this->i_active_edges);
	CollisionEdge* inv_e = edge_index == -1 ? nullptr : &this->edge_storage[edge_index];
	return inv_e;
}

bool CollisionGraph::collisionTwoSided(CollisionEdge* e)
{
	int opposite_edge_index = this->findEdgeIndex(e->m_i_adjacent, e->m_i_node, this->i_active_edges);
	if (opposite_edge_index != -1)
	{
		CollisionEdge* opposite_edge = &this->edge_storage[this->i_active_edges[opposite_edge_index]];
		// If the opposite Edge has a registered intersected_side, that means the collision is two-sided.
		return !(opposite_edge->m_side == INTERSECTED_SIDE::ODATA_NONE);
	}
	return false;
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
	// Calculate and store the bonus resolves for each CollisionEdge.
	for (int i = 0; i < c->i_m_primary.size(); i++)
	{
		CollisionEdge* e = &this->edge_storage[c->i_m_primary[i]];
		for (int j = 0; j < c->i_m_primary.size(); j++)
		{
			if (i == j) continue;
			CollisionEdge* e_tocheck = &this->edge_storage[c->i_m_primary[j]];
			// Check for resolvability of e for e_tocheck. Store e_tocheck in e.bonus if True.
			if (this->resolves(e, e_tocheck))
				e->i_m_bonus_resolves.push_back(e_tocheck->m_storage_index);
		}
	}
	
	// Sorting part, with a simple bubble sort implementation. The Edges are sorted in descending order
	// of bonus resolves.
	for (int i = 0; i < c->i_m_primary.size() - 1; i++)
	{
		for (int j = 0; j < c->i_m_primary.size() - i - 1; j++)
		{
			int current_index = c->i_m_primary[j];
			int current_size = this->edge_storage[current_index].i_m_bonus_resolves.size();
			int comparison_index = c->i_m_primary[j+1];
			int comparison_size = this->edge_storage[comparison_index].i_m_bonus_resolves.size();
			if (current_size < comparison_size)
			{
				c->i_m_primary[j] = comparison_index;
				c->i_m_primary[j + 1] = current_index;
			}
		}
	}
}

void CollisionGraph::sortAllPrimaryEdges()
{
	// For each Node, sort the Edges in descending order of bonus resolves.
	// Works correctly under assumption that sortPrimaryCollisions() have been run
	// before this.
	for (int index: this->i_primary_collisions)
	{
		// Exit function when the rest of the Nodes have < 2 Edges.
		if (this->node_storage[index].i_m_primary.size() < 2) return;
		this->sortEdges(&this->node_storage[index]);
	}
}

void CollisionGraph::sortPrimaryCollisions()
{
	// A simple insertion sort is executed below.
	for (int i = 1; i < this->i_primary_collisions.size(); i++)
	{
		int current_index = this->i_primary_collisions[i];
		int i_current_size = this->node_storage[current_index].i_m_primary.size();

		for (int j = i - 1; j > -2; j--)
		{
			if (j == -1)
			{
				this->i_primary_collisions[0] = current_index;
				break;
			}
			int comparison_index = this->i_primary_collisions[j];
			int i_comparison_size = this->node_storage[comparison_index].i_m_primary.size();
			if (i_current_size < i_comparison_size)
			{
				this->i_primary_collisions[j + 1] = current_index;
				break;
			}
			this->i_primary_collisions[j + 1] = comparison_index;
		}
	}
}

void CollisionGraph::clearVectors()
{
	this->node_storage.clear();
	this->i_primary_collisions.clear();
	this->i_triggered_collisions.clear();
	this->i_relaxed_collisions.clear();
	this->i_unresolved_collisions.clear();
	this->edge_storage.clear();
	this->i_active_edges.clear();
}

bool CollisionGraph::resolves(CollisionEdge* e, CollisionEdge* e_tocheck)
{
	return !(this->intersects(e, e_tocheck));
}

bool CollisionGraph::intersects(CollisionEdge* e, CollisionEdge* r)
{
	// Proposed resolve for collision e.
	sf::Vector2f e_pos = e->m_proposed_resolve;
	// Opposite object's position for collision r
	sf::Vector2f r_opposite_pos = this->node_storage[r->m_i_adjacent].m_updated_pos;

	// Object sizes.
	sf::Vector2f e_o_size = this->node_storage[e->m_i_node].m_node_object->getSize();
	sf::Vector2f r_o_size = this->node_storage[r->m_i_adjacent].m_node_object->getSize();

	// Intersection check.
	return (CollisionDetection::areIntersecting({ e_pos, e_o_size }, {r_opposite_pos, e_o_size}));
}
