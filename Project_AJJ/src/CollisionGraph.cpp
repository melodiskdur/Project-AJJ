#include "CollisionGraph.h"

CollisionGraph::CollisionGraph()
	: PhysicsAttribute("CollisionGraph")
{

}

CollisionGraph::~CollisionGraph()
{

}

void CollisionGraph::printVectors()
{
	
}

void CollisionGraph::resolveTree()
{
	this->printVectors();
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
		this->createNode(tuple.obj_i);
		this->createNode(tuple.obj_j);
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
		int c_index= this->createNode(holder.m_object);
		// Create Nodes, as well as Edges from holder.m_object to each close call object.
		for (Object* cc : holder.m_close_calls)
		{
			int cc_index = this->createNode(cc);
			this->createCloseCallEdge(this->findNode(c_index), this->findNode(cc_index));
		}
	}
}

int CollisionGraph::createNode(Object* object)
{
	// Double check that the Node doesn't already exist.
	if (this->findNode(object) != nullptr) return this->findNode(object)->m_storage_index;

	// Set up and store Node.
	CollisionNode new_node;
	new_node.m_node_object = object;
	new_node.m_frame_pos = object->getWorldPosition();
	new_node.m_updated_pos = new_node.m_frame_pos;
	new_node.m_storage_index = this->node_storage.size();

	// Node storage.
	this->node_storage.push_back(new_node);
	// Node index storage for later use.
	this->i_active_nodes.push_back(new_node.m_storage_index);
	return new_node.m_storage_index;
}

void CollisionGraph::createEdge(ObjectData& odata)
{
	CollisionNode* c_i = this->findNode(odata.m_object);
	CollisionNode* c_j = this->findNode(odata.m_colliding_object);
	// Exit if at least one Node is missing.
	if (c_i == nullptr || c_j == nullptr) return;
	// Setting up the new Edge.
	CollisionEdge new_edge;
	new_edge.m_i_node = c_i->m_storage_index;
	new_edge.m_i_adjacent = c_j->m_storage_index;
	new_edge.m_proposed_resolve = odata.m_wp;
	new_edge.m_storage_index = this->edge_storage.size();
	new_edge.m_side = odata.m_intersect;

	// Store Edge and add index to the storage to object's Node.primary.
	this->edge_storage.push_back(new_edge);
	this->i_active_edges.push_back(new_edge.m_storage_index);
	this->i_primary_edges.push_back(new_edge.m_storage_index);
	c_i->i_m_all.push_back(new_edge.m_storage_index);
	c_i->i_m_primary.push_back(new_edge.m_storage_index);
}

void CollisionGraph::createCloseCallEdge(CollisionNode* node, CollisionNode* adjacent)
{
	if (node == nullptr || adjacent == nullptr) return;
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
	node->i_m_all.push_back(new_edge.m_storage_index);
	node->i_m_relaxed.push_back(new_edge.m_storage_index);
}

void CollisionGraph::updatePosition(CollisionNode* c, CollisionEdge* e)
{
	
}

void CollisionGraph::recalibrateResolves(CollisionNode* c)
{

}

void CollisionGraph::handleTriggeredCollisions(CollisionNode* c, CollisionEdge* e, const std::vector<int>& i_triggered)
{
	
}

void CollisionGraph::collisionTriggered(CollisionEdge* r, CollisionEdge* e)
{
	
}

void CollisionGraph::markAsUnresolved(CollisionEdge* unres)
{
	
}

bool CollisionGraph::collisionTwoSided(CollisionEdge* e)
{
	return false;
}

bool CollisionGraph::resolves(CollisionEdge* e, CollisionEdge* e_tocheck)
{
	return !(this->intersects(e, e_tocheck));
}

bool CollisionGraph::intersects(CollisionEdge* e, CollisionEdge* r)
{
	CollisionNode* c = this->findNode(e->m_i_node);
	CollisionNode* adj = this->findNode(r->m_i_adjacent);
	if (c == nullptr || adj == nullptr) return false;
	// Proposed resolve.
	sf::Vector2f proposed = c->m_updated_pos + (e->m_proposed_resolve - c->m_updated_pos);
	if (CollisionDetection::areIntersecting({ proposed, c->m_node_object->getSize() }, { adj->m_updated_pos, adj->m_node_object->getSize() }))
		return true;
	return false;
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

int CollisionGraph::findNodeIndexIn(const int node_index, std::vector<int>& i_node_vector)
{
	for (int i = 0; i < i_node_vector.size(); i++)
	{
		if (node_index == i_node_vector[i]) return i;
	}
	return -1;
}

CollisionEdge* CollisionGraph::invertedEdge(CollisionEdge* e)
{
	int edge_index = this->findEdgeIndex(e->m_i_adjacent, e->m_i_node, this->i_active_edges);
	CollisionEdge* inv_e = edge_index == -1 ? nullptr : &this->edge_storage[edge_index];
	return inv_e;
}

CollisionNode* CollisionGraph::findNode(Object* o)
{
	for (CollisionNode& c: this->node_storage)
	{
		if (c.m_node_object == o) return (&c);
	}
	return nullptr;
}

CollisionNode* CollisionGraph::findNode(int node_index)
{
	for (CollisionNode& node : this->node_storage)
		if (node.m_storage_index == node_index) return (&this->node_storage[node_index]);
	return nullptr;
}

CollisionEdge* CollisionGraph::findEdge(int edge_index)
{
	for (CollisionEdge& edge : this->edge_storage)
		if (edge.m_storage_index == edge_index) return (&this->edge_storage[edge_index]);
	return nullptr;
}

std::vector<CollisionNode*> CollisionGraph::findOutgoingAdjacents(int node_index)
{
	return this->findOutgoingAdjacents(this->findNode(node_index));
}

std::vector<CollisionNode*> CollisionGraph::findOutgoingAdjacents(CollisionNode* c)
{
	std::vector<CollisionNode*> adjacents;
	CollisionNode* adj = nullptr;
	if (c != nullptr)
	{
		for (CollisionEdge* edge : this->findOutgoingEdges(c))
		{
			if ((adj = this->findNode(edge->m_i_adjacent)) != nullptr)
				adjacents.push_back(adj);
		}
	}
	return adjacents;
}

std::vector<CollisionNode*> CollisionGraph::findIncomingAdjacents(int node_index)
{
	return this->findIncomingAdjacents(findNode(node_index));
}

std::vector<CollisionNode*> CollisionGraph::findIncomingAdjacents(CollisionNode* c)
{
	std::vector<CollisionNode*> adjacents;
	CollisionNode* adj = nullptr;
	if (c != nullptr)
	{
		for (CollisionEdge* edge : this->findIncomingEdges(c))
		{
			if ((adj = findNode(edge->m_i_adjacent)) != nullptr) adjacents.push_back(adj);
		}
	}
	return adjacents;
}

std::vector<CollisionEdge*> CollisionGraph::findIncomingEdges(int node_index)
{
	return this->findIncomingEdges(this->findNode(node_index));
}

std::vector<CollisionEdge*> CollisionGraph::findIncomingEdges(CollisionNode* c)
{
	std::vector<CollisionEdge*> incoming;
	if (c != nullptr)
	{
		for (CollisionEdge& edge : this->edge_storage)
		{
			if (edge.m_i_adjacent == c->m_storage_index) incoming.push_back(&edge);
		}
	}
	return incoming;
}

std::vector<CollisionEdge*> CollisionGraph::findOutgoingEdges(int node_index)
{
	return this->findOutgoingEdges(this->findNode(node_index));
}

std::vector<CollisionEdge*> CollisionGraph::findOutgoingEdges(CollisionNode* c)
{
	std::vector<CollisionEdge*> outgoing;
	CollisionEdge* e = nullptr;
	if (c != nullptr)
	{
		for (int edge_index : c->i_m_all)
		{
			if ((e = this->findEdge(edge_index)) != nullptr) outgoing.push_back(e);
		}
	}
	return outgoing;
}

std::vector<CollisionEdge*> CollisionGraph::findOutgoingEdgesByStatus(CollisionNode* c, EDGE_STATUS status)
{
	std::vector<CollisionEdge*> edges;
	CollisionEdge* edge = nullptr;
	std::vector<int>* edge_vector = nullptr;
	switch (status)
	{
	case EDGE_STATUS::EDGE_ACTIVE: edge_vector = &(c->i_m_primary); break;
	case EDGE_STATUS::EDGE_RELAXED: edge_vector = &(c->i_m_relaxed); break;
	case EDGE_STATUS::EDGE_TRIGGERED: edge_vector = &(c->i_m_triggered); break;
	case EDGE_STATUS::EDGE_UNRESOLVED: edge_vector = &(c->i_m_unresolved); break;
	}
	if (edge_vector != nullptr)
	{
		for (int& edge_index : *edge_vector)
		{
			if ((edge = findEdge(edge_index)) != nullptr) edges.push_back(edge);
		}
	}
	return edges;
}

void CollisionGraph::moveEdgeIndex(CollisionEdge* e, std::vector<int>& from, std::vector<int>& to)
{
	int index;
	if (index = this->findEdgeIndex(e->m_i_node, e->m_i_adjacent, from) != -1)
	{
		from.erase(from.begin() + index);
		to.push_back(e->m_storage_index);
	}
}

void CollisionGraph::moveNodeIndex(CollisionNode* c, std::vector<int>& from, std::vector<int>& to)
{
	int index;
	if (index = this->findNodeIndexIn(c->m_storage_index, from) != -1)
	{
		from.erase(from.begin() + index);
		to.push_back(c->m_storage_index);
	}
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
	for (int index: this->i_active_nodes)
	{
		// Exit function when the rest of the Nodes have < 2 Edges.
		if (this->node_storage[index].i_m_primary.size() < 2) return;
		this->sortEdges(&this->node_storage[index]);
	}
}

void CollisionGraph::sortPrimaryCollisions()
{
	// A simple insertion sort is executed below.
	for (int i = 1; i < this->i_active_nodes.size(); i++)
	{
		int current_index = this->i_active_nodes[i];
		int i_current_size = this->node_storage[current_index].i_m_primary.size();

		for (int j = i - 1; j > -2; j--)
		{
			if (j == -1)
			{
				this->i_active_nodes[0] = current_index;
				break;
			}
			int comparison_index = this->i_active_nodes[j];
			int i_comparison_size = this->node_storage[comparison_index].i_m_primary.size();
			if (i_current_size < i_comparison_size)
			{
				this->i_active_nodes[j + 1] = current_index;
				break;
			}
			this->i_active_nodes[j + 1] = comparison_index;
		}
	}
}

void CollisionGraph::clearVectors()
{
	this->node_storage.clear();
	this->i_active_nodes.clear();
	this->edge_storage.clear();
	this->i_active_edges.clear();
	this->i_primary_edges.clear();
	this->i_relaxed_edges.clear();
	this->i_triggered_edges.clear();
	this->i_unresolved_edges.clear();
}

std::vector<sf::VertexArray> CollisionGraph::getTree()
{
	std::vector<sf::VertexArray> edges;
	for (CollisionEdge& e : this->edge_storage)
	{
		CollisionNode* c = findNode(e.m_i_node);
		CollisionNode* adj = findNode(e.m_i_adjacent);
		sf::VertexArray edge(sf::Lines, 2);
		edge[0].position = (c->m_updated_pos + 0.5f * (c->m_node_object->getSize()));
		edge[1].position = (adj->m_updated_pos + 0.5f * (adj->m_node_object->getSize()));
		sf::Color gc = e.m_is_relaxed ? sf::Color::Blue : sf::Color::Red;
		edge[0].color = gc;
		edge[1].color = gc;
		edges.push_back(edge);
	}
	return edges;
}
