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
	//std::cout << "========================================\n";
	this->sortPrimaryEdgesByType();
	for (CollisionEdge* e : this->findEdgesByVector(this->i_primary_edges))
	{
		if (e->m_is_relaxed) continue;
		CollisionNode* c = this->findNode(e->m_i_node);
		this->resolveCollision(e, c);
		if (this->collisionTwoSided(e)) this->resolveCollision(this->invertedEdge(e), this->findNode(e->m_i_adjacent));
	}
	while (this->i_triggered_edges.size() > 0)
	{
		std::swap(this->i_primary_edges, this->i_triggered_edges);
		for (CollisionEdge* e : this->findEdgesByVector(this->i_primary_edges))
		{
			CollisionNode* c = this->findNode(e->m_i_node);
			std::swap(c->i_m_primary, c->i_m_triggered);
			this->resolveCollision(e, c);
		}
	}
	
	// Check for intersects.
	for (CollisionEdge* e : findEdgesByVector(this->i_active_edges))
	{
		CollisionNode* c = this->findNode(e->m_i_node);
		CollisionNode* c_adj = this->findNode(e->m_i_adjacent);
		if (this->intersects(c, c_adj))
		{
			/*
			std::cout << "REMAINING INTERSECT (" << c->m_node_object->getId() << ", "
				<< c_adj->m_node_object->getId() << ") [ " << std::string(this->intersectString(e->m_side))
				<< " ] [ " << e->m_is_visited << " ]\n";
		    */
		}
	}
}

void CollisionGraph::resolveCollision(CollisionEdge* e, CollisionNode* c)
{
	std::vector<int> i_triggered;
	// Check for triggered collisions.
	for (CollisionEdge* r : this->findOutgoingEdgesByStatus(c, EDGE_STATUS::EDGE_RELAXED))
	{
		// If the resolve for Edge e triggers Edge r, we store r.index in i_triggeed.
		if (this->intersects(e, r))
		{
			if (this->staticTriggersStatic(e, r))
			{
			//	// PRINT
			//	std::cout << "STATIC TO STATIC-TRIGGER\n";
			//	// /PRINT
				this->moveEdgeIndex(e, this->i_primary_edges, this->i_unresolved_edges);
				this->moveEdgeIndex(e, c->i_m_primary, c->i_m_unresolved);
				return;
			}
			i_triggered.push_back(r->m_storage_index);
		}
	}
	// Check for bonus resolves.
	for (CollisionEdge* e_bonus : this->findOutgoingEdgesByStatus(c, EDGE_STATUS::EDGE_ACTIVE))
	{
		// Skip if we're looking at the same Edge.
		if (e_bonus->m_storage_index == e->m_storage_index) continue;
		// If we find a bonus resolve, store it's storage index in e.bonus_resolves.
		if (this->resolves(e, e_bonus)) e->i_m_bonus_resolves.push_back(e_bonus->m_storage_index);
	}

	this->updatePosition(c, e);
	this->handleTriggeredCollisions(c, e, i_triggered);
	this->recalibrateResolves(c);
}

void CollisionGraph::applyTreeResolution()
{
	for (CollisionNode& node : this->node_storage)
	{
		node.m_hbox->updateObjectPos(node.m_updated_pos);
		// DEBUGGING
		// THIS SHOULD PERHAPS NOT BE HERE.
		sf::Vector2f o_velocity = node.m_node_object->getVelocity();
		//if (node.m_horizontal_collision) o_velocity.x = 0.0f;
		if (node.m_vertical_collision) o_velocity.y = 0.0f;
		node.m_node_object->setVelocity(o_velocity);
		// END DEBUGGING.
	}
}

void CollisionGraph::storeCollisions(std::vector<CollTuple> collision_tuples)
{
	this->clearVectors();
	for (CollTuple tuple : collision_tuples)
	{
		this->createNode(tuple.m_ni);
		this->createNode(tuple.m_nj);
		// Calculate CollisionHandler resolves and create the corresponding CollisionEdges.
		std::vector<HBData> v_odata = CollisionHandler::separateHitboxes(tuple.m_ni, tuple.m_nj);
		for (HBData odata : v_odata)
		{
			this->createEdge(odata);
		}
	}
}

void CollisionGraph::storeCloseCalls(std::vector<CCHolder>& close_calls)
{
	for (CCHolder holder : close_calls)
	{
		int c_index = this->createNode(holder.m_n);
		// Create Nodes, as well as Edges from holder.m_object to each close call object.
		for (HitboxNode* cc : holder.m_cc)
		{
			int cc_index = this->createNode(cc);
			this->createCloseCallEdge(this->findNode(c_index), this->findNode(cc_index));
		}
	}
}

int CollisionGraph::createNode(HitboxNode* hb)
{
	// Double check that the Node doesn't already exist.
	if (this->findNode(hb->getParentObject()) != nullptr) return this->findNode(hb->getParentObject())->m_storage_index;
	// Set up and store Node.
	CollisionNode new_node;
	new_node.m_node_object = hb->getParentObject();
	new_node.m_hbox = hb;
	new_node.m_frame_pos = hb->getGlobalPos();
	new_node.m_updated_pos = new_node.m_frame_pos;
	new_node.m_storage_index = static_cast<int>(this->node_storage.size());
	// Node storage.
	this->node_storage.push_back(new_node);
	// Node index storage for later use.
	this->i_active_nodes.push_back(new_node.m_storage_index);
	return new_node.m_storage_index;
}

void CollisionGraph::createEdge(HBData& odata)
{
	CollisionNode* c_i = this->findNode(odata.m_this->getParentObject());
	CollisionNode* c_j = this->findNode(odata.m_other->getParentObject());
	// Exit if at least one Node is missing.
	if (c_i == nullptr || c_j == nullptr) return;
	// Setting up the new Edge.
	CollisionEdge new_edge;
	new_edge.m_i_node = c_i->m_storage_index;
	new_edge.m_i_adjacent = c_j->m_storage_index;
	new_edge.m_proposed_resolve = odata.m_this_resolve;
	new_edge.m_storage_index = static_cast<int>(this->edge_storage.size());
	new_edge.m_side = odata.m_this_side;

	// DEBUGGING.
	new_edge.oi_id = c_i->m_node_object->getId();
	new_edge.oj_id = c_j->m_node_object->getId();
	// DEBUGGING.

	// Store Edge and add index to the storage to object's Node.primary.
	this->edge_storage.push_back(new_edge);
	this->i_active_edges.push_back(new_edge.m_storage_index);
	this->i_primary_edges.push_back(new_edge.m_storage_index);
	c_i->i_m_all.push_back(new_edge.m_storage_index);
	c_i->i_m_primary.push_back(new_edge.m_storage_index);
}

void CollisionGraph::createInvertedEdge(CollisionEdge* e)
{
	CollisionNode* i = this->findNode(e->m_i_adjacent);
	CollisionNode* j = this->findNode(e->m_i_node);
	this->createCloseCallEdge(i, j);
}

void CollisionGraph::createCloseCallEdge(CollisionNode* node, CollisionNode* adjacent)
{
	if (node == nullptr || adjacent == nullptr) return;
	// Setting up the new Edge.
	CollisionEdge new_edge;
	new_edge.m_i_node = node->m_storage_index;
	new_edge.m_i_adjacent = adjacent->m_storage_index;
	new_edge.m_proposed_resolve = node->m_updated_pos;
	new_edge.m_storage_index = static_cast<int>(this->edge_storage.size());
	new_edge.m_side = INTERSECTED_SIDE::ODATA_NONE;
	// Set edge as relaxed.
	new_edge.m_is_relaxed = true;

	// DEBUGGING.
	new_edge.oi_id = node->m_node_object->getId();
	new_edge.oj_id = adjacent->m_node_object->getId();
	// DEBUGGING.

	// Store Edge and add index to the storage to object's Node.relaxed.
	this->edge_storage.push_back(new_edge);
	this->i_active_edges.push_back(new_edge.m_storage_index);
	this->i_relaxed_edges.push_back(new_edge.m_storage_index);
	node->i_m_all.push_back(new_edge.m_storage_index);
	node->i_m_relaxed.push_back(new_edge.m_storage_index);
}

void CollisionGraph::updatePosition(CollisionNode* c, CollisionEdge* e)
{
	if (e->m_proposed_resolve.x == REPOS_INFINITY ||e->m_proposed_resolve.y == REPOS_INFINITY) return;
	// Update phase. Set Edge to relaxed and move it to c.relaxed AND graph.relaxed.
	c->m_updated_pos += (e->m_proposed_resolve - c->m_updated_pos);
	e->m_is_relaxed = true;
	// PRINT
	//std::cout << "RESOLVED (" << c->m_node_object->getId() << ", " << this->findNode(e->m_i_adjacent)->m_node_object->getId() << ") [ "
	//	<< std::string(this->intersectString(e->m_side)) << " ]\n";
	// /PRINT
	if (this->findNode(e->m_i_adjacent)->m_hbox->getBehavior() == HBOX::STATIC)
		e->m_is_visited = true;
	this->moveEdgeIndex(e, c->i_m_primary, c->i_m_relaxed);
	this->moveEdgeIndex(e, this->i_primary_edges, this->i_relaxed_edges);
	// Bonus phase. Do the same thing.
	for (CollisionEdge* e_bonus : this->findEdgesByVector(e->i_m_bonus_resolves))
	{
		e_bonus->m_is_relaxed = true;
		this->moveEdgeIndex(e_bonus, c->i_m_primary, c->i_m_relaxed);
		this->moveEdgeIndex(e_bonus, this->i_primary_edges, this->i_relaxed_edges);
	//	// PRINT
	//	std::cout << "BONUS RESOLVE (" << c->m_node_object->getId() << ", " << this->findNode(e_bonus->m_i_adjacent)->m_node_object->getId() << ") [ "
	//		<< std::string(this->intersectString(e_bonus->m_side)) << " ]\n";
		// /PRINT
	}
	e->i_m_bonus_resolves.clear();
	// (NOTE: THIS MIGHT BE TEMPORARY) Set collided axis.
	switch (e->m_side)
	{
	case INTERSECTED_SIDE::ODATA_TOP: c->m_vertical_collision = true; break;
	case INTERSECTED_SIDE::ODATA_BOTTOM: c->m_vertical_collision = true; break;
	case INTERSECTED_SIDE::ODATA_LEFT: c->m_horizontal_collision = true; break;
	case INTERSECTED_SIDE::ODATA_RIGHT: c->m_horizontal_collision = true; break;
	}
}

void CollisionGraph::recalibrateResolves(CollisionNode* c)
{
	for (CollisionEdge* e : this->findOutgoingEdges(c))
	{
		// No recalibration needed if if an Edge is relaxed.
		HitboxNode* adj = this->findNode(e->m_i_adjacent)->m_hbox;
		if (e->m_is_relaxed) continue;
		if (adj->getSB_pts().size() > 0) continue;
		HBData temp_data;
		temp_data.m_this = c->m_hbox;
		temp_data.m_other = adj;
		temp_data.m_this_side = e->m_side;
		// Recalibrations. Look for the inverted Edge to determine if there's a double collision at hand. If
		// there is a double collision but the inverted Edge is visited (it's been triggered in a collision
		// chain originating from a STATIC object), we proceed as if this is a single collision (only moving c.object.)
		CollisionEdge* e_inv = this->invertedEdge(e);
		INTERSECTED_SIDE adj_intersect;
		if (e_inv != nullptr)
			adj_intersect = e_inv->m_is_visited ? INTERSECTED_SIDE::ODATA_NONE : e_inv->m_side;
		else
			adj_intersect = INTERSECTED_SIDE::ODATA_NONE;
		std::vector<sf::Vector2f> recab_repos = CollisionHandler::recalibrate(temp_data, c->m_updated_pos, adj_intersect, this->findNode(e->m_i_adjacent)->m_updated_pos);
		// Update Edges' proposed resolves.

		//std::cout << "RECALIBRATE (" << c->m_node_object->getId() << ", " << this->findNode(e->m_i_adjacent)->m_node_object->getId()
		//	<< ")  :  " << "[ " << e->m_proposed_resolve.x << " " << e->m_proposed_resolve.y << " ] -> [ "
		//	<< recab_repos[0].x << " " << recab_repos[0].y << " ]\n";

		e->m_proposed_resolve = recab_repos[0];
		if (e_inv != nullptr) e_inv->m_proposed_resolve = recab_repos[1];
	}
}

void CollisionGraph::handleTriggeredCollisions(CollisionNode* c, CollisionEdge* e, const std::vector<int>& i_triggered)
{
	for (CollisionEdge* e_trig : this->findEdgesByVector(i_triggered))
	{
		this->collisionTriggered(e_trig, e);
	}
}

void CollisionGraph::collisionTriggered(CollisionEdge* r, CollisionEdge* e)
{
	// Setup part.
	CollisionNode* r_adj = this->findNode(r->m_i_adjacent);
	if (r_adj == nullptr) return;
	CollisionNode* r_c = this->findNode(r->m_i_node);
	CollisionEdge* r_inv = this->invertedEdge(r);
	bool chain_case = false;
	if (r_inv == nullptr)
	{
		int i_e = e->m_storage_index;
		this->createInvertedEdge(r);
		r_inv = this->findEdge(static_cast<int>(this->edge_storage.size()) - 1);
		r = this->invertedEdge(r_inv);
		e = this->findEdge(i_e);
	}
	if (r->m_side == INTERSECTED_SIDE::ODATA_NONE)
		r->m_side = this->oppositeSide(e->m_side);

	if (r_adj->m_hbox->getBehavior() == HBOX::STATIC)
		chain_case = true;
	else
	{
		for (CollisionEdge* r_adj_e : this->findOutgoingEdges(r_adj))
		{
			// If we find an edge from r.adjacent in the same direction as
			// r that is visited, this means we've found a chain leading to a
			// a STATIC collision. This is not permitted.
			if ((r_adj_e->m_side == r->m_side) && r_adj_e->m_is_visited)
			{
				chain_case = true;
				break;
			}
		}
	}
	// Case 2 (Chain case).
	if (chain_case)
	{
		// Set r.isvisited to true, as a marker that that this
			// Edge is part of a chain to a STATIC collision.
		r->m_is_visited = true;
		r->m_is_relaxed = false;
		// We set the inverted_r.side to NONE, indicating that
		// only r.m_node will be moved when we recalibrate the resolve.
		r_inv->m_side = INTERSECTED_SIDE::ODATA_NONE;
		// Move r to the 'triggered'-index vectors.
		// PRINT
		//std::cout << "TRIGGERED (" << r_c->m_node_object->getId()
		//	<< ", " << r_adj->m_node_object->getId() << ") [ "
		//	<< std::string(this->intersectString(r->m_side)) << " ]\n";
		// /PRINT
		this->moveEdgeIndex(r, r_c->i_m_relaxed, r_c->i_m_triggered);
		this->moveEdgeIndex(r, this->i_relaxed_edges, this->i_triggered_edges);
		this->recalibrateResolves(r_c);
	}
	// Case 1 (Normal case).
	// If e resolved a collison with a STATIC Node, mark r_inv (it has
	// the same direction as e) as visited to indicate it's connected to a 
	// chain to a STATIC collision.
	else if (this->findEdgeIndex(r_inv->m_i_node, r_inv->m_i_adjacent, r_adj->i_m_relaxed) != -1)
	{
		r_inv->m_is_visited = e->m_is_visited;
		r_inv->m_is_relaxed = false;
		r_inv->m_side = this->oppositeSide(r->m_side);
		r->m_side = INTERSECTED_SIDE::ODATA_NONE;
		// PRINT
		//std::cout << "INV TRIGGERED (" << r_adj->m_node_object->getId()
		//	<< ", " << r_c->m_node_object->getId() << ") [ "
		//	<< std::string(this->intersectString(r_inv->m_side)) << " ]\n";
		// /PRINT
		// Move r_inv to the 'triggered'-index vectors.
		this->moveEdgeIndex(r_inv, r_adj->i_m_relaxed, r_adj->i_m_triggered);
		this->moveEdgeIndex(r_inv, this->i_relaxed_edges, this->i_triggered_edges);
		this->recalibrateResolves(r_adj);
	}
}

void CollisionGraph::markAsUnresolved(CollisionEdge* unres)
{
	
}

bool CollisionGraph::collisionTwoSided(CollisionEdge* e)
{
	CollisionEdge* e_inv = this->invertedEdge(e);
	if (e_inv != nullptr && e_inv->m_side != INTERSECTED_SIDE::ODATA_NONE)
		return true;
	return false;
}

bool CollisionGraph::staticTriggersStatic(CollisionEdge* e, CollisionEdge* r)
{
	CollisionNode* e_adj = this->findNode(e->m_i_adjacent);
	CollisionNode* r_adj = this->findNode(r->m_i_adjacent);
	return (e_adj->m_hbox->getBehavior() == HBOX::STATIC &&
		r_adj->m_hbox->getBehavior() == HBOX::STATIC);
}

bool CollisionGraph::resolves(CollisionEdge* e, CollisionEdge* e_tocheck)
{
	// First, check that they're intersecting before moving.
	CollisionNode* c = this->findNode(e->m_i_node);
	CollisionNode* adj = this->findNode(e_tocheck->m_i_adjacent);
	sf::Vector2f sz_c = { c->m_hbox->getBB().width, c->m_hbox->getBB().height };
	sf::Vector2f sz_adj = { adj->m_hbox->getBB().width, adj->m_hbox->getBB().height };
	if (CollisionHandler::bb_intersects({ c->m_updated_pos, sz_c }, { adj->m_updated_pos, sz_adj }))
	{
		return !(this->intersects(e, e_tocheck));
	}
	return false;
}

bool CollisionGraph::intersects(CollisionEdge* e, CollisionEdge* r)
{
	CollisionNode* c = this->findNode(e->m_i_node);
	CollisionNode* adj = this->findNode(r->m_i_adjacent);
	if (c == nullptr || adj == nullptr) return false;
	// Proposed resolve.
	sf::Vector2f proposed = c->m_updated_pos + (e->m_proposed_resolve - c->m_updated_pos);
	// Sizes.
	sf::Vector2f sz_c = { c->m_hbox->getBB().width, c->m_hbox->getBB().height };
	sf::Vector2f sz_adj = { adj->m_hbox->getBB().width, adj->m_hbox->getBB().height };
	if (CollisionHandler::bb_intersects({ proposed, sz_c }, {adj->m_updated_pos, sz_adj}))
		return true;
	return false;
}

bool CollisionGraph::intersects(CollisionNode * c, CollisionNode* c_adj)
{
	sf::Vector2f sz_c = { c->m_hbox->getBB().width, c->m_hbox->getBB().height };
	sf::Vector2f sz_adj = { c_adj->m_hbox->getBB().width, c_adj->m_hbox->getBB().height };
	return CollisionHandler::bb_intersects({ c->m_updated_pos, sz_c }, { c_adj->m_updated_pos, sz_adj });
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
	std::vector<int> edge_vector;
	switch (status)
	{
	case EDGE_STATUS::EDGE_ACTIVE: edge_vector = (c->i_m_primary); break;
	case EDGE_STATUS::EDGE_RELAXED: edge_vector = (c->i_m_relaxed); break;
	case EDGE_STATUS::EDGE_TRIGGERED: edge_vector = (c->i_m_triggered); break;
	case EDGE_STATUS::EDGE_UNRESOLVED: edge_vector = (c->i_m_unresolved); break;
	}
	for (int edge_index : edge_vector)
	{
		if ((edge = this->findEdge(edge_index)) != nullptr) edges.push_back(edge);
	}
	return edges;
}

std::vector<CollisionEdge*> CollisionGraph::findEdgesByVector(const std::vector<int>& edge_indices)
{
	std::vector<CollisionEdge*> edges;
	CollisionEdge* edge;
	for (int edge_index : edge_indices)
		if ((edge = this->findEdge(edge_index)) != nullptr) edges.push_back(edge);
	return edges;
}

void CollisionGraph::moveEdgeIndex(CollisionEdge* e, std::vector<int>& from, std::vector<int>& to)
{
	int index;
	if ((index = this->findEdgeIndex(e->m_i_node, e->m_i_adjacent, from)) != -1)
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

void CollisionGraph::sortPrimaryEdgesByType()
{
	std::vector<int> temp;
	// Pop each Edge. If both objects are DYNAMIC, push to front. Else, push to back.
	for (int index : this->i_primary_edges)
	{
		CollisionEdge* e = this->findEdge(index);
		CollisionNode* c = this->findNode(e->m_i_node);
		CollisionNode* adj = this->findNode(e->m_i_adjacent);
		if (c->m_hbox->getBehavior() == HBOX::DYNAMIC &&
			adj->m_hbox->getBehavior() == HBOX::DYNAMIC)
			temp.insert(temp.begin(), e->m_storage_index);
		else temp.insert(temp.end(), e->m_storage_index);
	}
	// Swap the contents of temp and g.i_primary.
	this->i_primary_edges = temp;
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