#include "CollisionDetection.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * *
*					CollisionDetection             *
 * * * * * * * * * * * * * * * * * * * * * * * * * */
CollisionDetection::CollisionDetection(std::vector<Object*>& objects):
	PhysicsAttribute("Collision Detection")
{
	scene_objects = &objects;
	root = new QuadTree();

	hb_root = new QuadTree_Hitbox();

	// Debugging
	root->setGridVector(&this->grid_lines);
;}

CollisionDetection::~CollisionDetection()
{
	delete root;
}

std::vector<ObjectTuple> CollisionDetection::getCollisions(sf::FloatRect view_rect)
{
	std::vector<Object*> collision_candidates;
	std::vector<Object*> rendered_objects;
	this->holders.clear();
	this->collisions.clear();
	this->collisions.resize(0);
	root->clearTree();
	root->setRootBoundaries(view_rect);
	for (int i = 0; i < scene_objects->size(); i++)
	{
		//Collision detection only needs to be done for objects within the camera's view.
		if (view_rect.contains(scene_objects->at(i)->getWorldPosition()))
		{
			rendered_objects.push_back(scene_objects->at(i));
			root->insertObject(scene_objects->at(i));
		}
	}

	for (int i = 0; i < rendered_objects.size(); i++)
	{
		//Collision detection only needs to be done for objects within the camera's view.
		if (rendered_objects[i]->getBehaviorType() == OBJECT_BEHAVIOR::STATIC) continue;

		// Create a CloseCallsHolder storage for close call objects to object i.
		CloseCallHolder i_holder;
		i_holder.m_object = rendered_objects[i];
		i_holder.m_close_calls.clear();

		collision_candidates.clear();
		collision_candidates = root->getCollisionCandidates(rendered_objects.at(i));
		sf::FloatRect object_i = sf::FloatRect(rendered_objects.at(i)->getWorldPosition(), rendered_objects.at(i)->getSize());		//Object i's collision box.
		for (int j = 0; j < collision_candidates.size(); j++)
		{
			sf::FloatRect object_j = sf::FloatRect(rendered_objects.at(j)->getWorldPosition(), rendered_objects.at(j)->getSize());  //Object j's collision box.
			if ( this->areIntersecting(object_i, object_j) &&																		//Intersection.
				rendered_objects.at(i) != rendered_objects.at(j) &&																	//Not the same object.
				!this->tupleExists(rendered_objects.at(i), rendered_objects.at(j)) )  												//Collision is not already registered in vector.
			{
				ObjectTuple new_collision;
				new_collision.obj_i = rendered_objects.at(i);
				new_collision.obj_j = rendered_objects.at(j);
				this->collisions.push_back(new_collision);
			}
			// If no collision is done, we store the object in object i:s candidate holder.
			else if (rendered_objects.at(i) != rendered_objects.at(j) && this->isCloseCall(rendered_objects.at(i), rendered_objects.at(j)) &&
				    !this->tupleExists(rendered_objects.at(i), rendered_objects.at(j)))
			{
				i_holder.m_close_calls.push_back(rendered_objects.at(j));
			}
		}
		// Add object i:s close calls to vector.
		if (i_holder.m_close_calls.size() > 0) this->holders.push_back(i_holder);
	}
	return this->collisions;
}

std::vector<CollTuple> CollisionDetection::getCollisions_CT(sf::FloatRect view_rect)
{
	std::vector<HitboxNode*> collision_candidates;
	std::vector<HitboxNode*> rendered_objects;
	this->cc_holders.clear();
	this->coll_tuples.clear();
	hb_root->clearTree();
	hb_root->setRootBoundaries(view_rect);
	for (HitboxNode* hbn : this->hitboxes)
	{
		//Collision detection only needs to be done for objects within the camera's view.
		if (view_rect.intersects(hbn->getBB()))
		{
			rendered_objects.push_back(hbn);
			hb_root->insertObject(hbn);
		}
	}

	for (HitboxNode* hbn : rendered_objects)
	{
		// Skip STATIC objects such as ground, etc.
		if (hbn->getBehavior() == HBOX::STATIC) continue;

		// Create a CloseCallsHolder storage for close call objects to object i.
		CCHolder i_holder;
		i_holder.m_n = hbn;
		i_holder.m_cc.clear();

		collision_candidates.clear();
		collision_candidates = hb_root->getCollisionCandidates(hbn);
		sf::FloatRect object_i = hbn->getBB();		//Object i's collision box.
		for (HitboxNode* hbn_cand : collision_candidates)
		{
			sf::FloatRect object_j = hbn_cand->getBB();  //Object j's collision box.
			if (this->areIntersecting(object_i, object_j) &&																		//Intersection.
				hbn != hbn_cand &&																	//Not the same object.
				!this->tupleExistsX(hbn, hbn_cand))  												//Collision is not already registered in vector.
			{
				CollTuple new_collision;
				new_collision.m_ni = hbn;
				new_collision.m_nj = hbn_cand;
				this->coll_tuples.push_back(new_collision);
			}
			// If no collision is done, we store the object in object i:s candidate holder.
			else if (hbn != hbn_cand && this->isCloseCallX(hbn, hbn_cand) &&
				!this->tupleExistsX(hbn, hbn_cand))
			{
				i_holder.m_cc.push_back(hbn_cand);
			}
		}
		// Add object i:s close calls to vector.
		if (i_holder.m_cc.size() > 0) this->cc_holders.push_back(i_holder);
	}
	return (this->coll_tuples);
}

void CollisionDetection::checkForCollisions(sf::FloatRect view_rect)
{
	std::vector<Object*> collision_candidates;
	std::vector<Object*> rendered_objects;
	this->collisions.clear();
	this->collisions.resize(0);
	root->clearTree();
	root->setRootBoundaries(view_rect);
	for (int i = 0; i < scene_objects->size(); i++)
	{
		//Collision detection only needs to be done for objects within the camera's view.
		if (view_rect.contains(scene_objects->at(i)->getWorldPosition()))
		{
			rendered_objects.push_back(scene_objects->at(i));
			root->insertObject(scene_objects->at(i));
		}
	}

	for (int i = 0; i < rendered_objects.size(); i++)
	{
		collision_candidates.clear();
		collision_candidates = root->getCollisionCandidates(rendered_objects.at(i));
		sf::FloatRect object_i = sf::FloatRect(rendered_objects.at(i)->getWorldPosition(), rendered_objects.at(i)->getSize());		//Object i's collision box.
		for (int j = 0; j < collision_candidates.size(); j++)
		{
			sf::FloatRect object_j = sf::FloatRect(rendered_objects.at(j)->getWorldPosition(), rendered_objects.at(j)->getSize());  //Object j's collision box.
			if (this->areIntersecting(object_i, object_j) &&																		//Intersection.
				rendered_objects.at(i) != rendered_objects.at(j) &&																	//Not the same object.
				!this->tupleExists(rendered_objects.at(i), rendered_objects.at(j)) &&												//Collision is not already registered in vector.
				this->atLeastOneMoving(rendered_objects.at(i), rendered_objects.at(j)))											//At least one object is moving.
			{
				ObjectTuple new_collision;
				new_collision.obj_i = rendered_objects.at(i);
				new_collision.obj_j = rendered_objects.at(j);
				this->collisions.push_back(new_collision);
			}
		}
	}
	this->handleCollisions();
}

bool CollisionDetection::tupleExists(Object* o_i, Object* o_j)
{
	for (int i = 0; i < this->collisions.size(); i++)
	{
		if ((collisions[i].obj_i == o_i && collisions[i].obj_j == o_j) ||
			(collisions[i].obj_i == o_j && collisions[i].obj_j == o_i))
			return true;
	}
	return false;
}

bool CollisionDetection::tupleExistsX(HitboxNode* n_i, HitboxNode* n_j)
{
	std::vector<CollTuple>::iterator ct_it = std::find_if(this->coll_tuples.begin(), this->coll_tuples.end(),
		[&](CollTuple ct) -> bool
		{ 
			return ((ct.m_ni == n_i && ct.m_nj == n_j) || (ct.m_ni == n_j && ct.m_nj == n_i));
		});
	return (ct_it != this->coll_tuples.end());
}

void CollisionDetection::handleCollisions()
{
	for (int i = 0; i < this->collisions.size(); i++)
	{
		//Collision handling
		CollisionHandler::separateCollisionHandleres(collisions[i].obj_i, collisions[i].obj_j);
		this->removeResolved();
	}
}

bool CollisionDetection::atLeastOneMoving(Object* i, Object* j)
{
	sf::Vector2f ivel = i->getVelocity();
	sf::Vector2f jvel = j->getVelocity();
	// True if at least one object is moving in at least one direction.
	return (ivel.x != 0 || ivel.y != 0 || jvel.x != 0 || jvel.y != 0);
}

bool CollisionDetection::areIntersecting(sf::FloatRect ibox, sf::FloatRect jbox)
{
	return (ibox.intersects(jbox) || jbox.intersects(ibox));
}

bool CollisionDetection::isCloseCall(Object* i, Object* j)
{
	// Define a FloatRect representing the proximity of i.
	sf::Vector2f i_prox_topleft = i->getWorldPosition() - 0.5f * i->getSize();
	sf::Vector2f i_prox_size = i->getSize() * 2.5f;
	sf::FloatRect i_prox_rect = {i_prox_topleft, i_prox_size};

	// Close object CollisionHandler.
	sf::FloatRect j_box = { j->getWorldPosition(), j->getSize() };
	// Call to ColDet::areIntersecting.
	return (CollisionDetection::areIntersecting(i_prox_rect, j_box));
}

bool CollisionDetection::isCloseCallX(HitboxNode* n_i, HitboxNode* n_j)
{
	sf::FloatRect bb_i = n_i->getBB();
	sf::FloatRect bbcc_i = { {bb_i.left - 0.5f * bb_i.width, bb_i.top - 0.5f * bb_i.height}, {2.5f * bb_i.width, 2.5f * bb_i.height} };
	sf::FloatRect bb_j = n_j->getBB();
	return (CollisionDetection::areIntersecting(bbcc_i, bb_j));
}

void CollisionDetection::removeResolved()
{
	for (int i = 0; i < this->collisions.size(); i++)
	{
		if ( !areIntersecting(sf::FloatRect(collisions[i].obj_i->getWorldPosition(), collisions[i].obj_i->getSize()),
			sf::FloatRect(collisions[i].obj_j->getWorldPosition(), collisions[i].obj_j->getSize())) )
		{
			collisions.erase(collisions.begin() + i);
			i = 0;
		}
	}
}

void CollisionDetection::findHitboxes()
{
	// Exit if the Collision Detection instance isn't connected to a Scene.
	if (this->scene_objects == nullptr) return;
	// Search each Object for a HitboxNode.
	for (Object* o : *(this->scene_objects))
	{
		std::vector<PropertyNode*> ns = o->getPropertyNodes();
		// Search for the first HitboxNode-instance in the property_nodes-vector of o.
		std::vector<PropertyNode*>::iterator ns_it = std::find_if(ns.begin(), ns.end(),
			[](PropertyNode* pn) -> bool {return (pn->getPropertyName() == sf::String("Hitbox"));});
		// If found: cast from PropertyNode to HitboxNode and store.
		if (ns_it != ns.end())
			this->hitboxes.push_back(static_cast<HitboxNode*>(*ns_it));
	}
}

// Debugging.
std::vector<sf::VertexArray> CollisionDetection::getGrid()
{
	std::vector<sf::VertexArray> grid;
	sf::Color gc = sf::Color::Red;
	for (int i = 0; i < this->grid_lines.size(); i++)
	{
		sf::VertexArray subgrid(sf::LineStrip, 4);
		subgrid[0] = sf::Vector2f(this->grid_lines[i].left, this->grid_lines[i].top);
		subgrid[1] = sf::Vector2f(this->grid_lines[i].left + this->grid_lines[i].width, this->grid_lines[i].top);
		subgrid[2] = sf::Vector2f(this->grid_lines[i].left + this->grid_lines[i].width, this->grid_lines[i].top + this->grid_lines[i].height);
		subgrid[3] = sf::Vector2f(this->grid_lines[i].left, this->grid_lines[i].top + this->grid_lines[i].height);
		subgrid[0].color = gc;
		subgrid[1].color = gc;
		subgrid[2].color = gc;
		subgrid[3].color = gc;
		grid.push_back(subgrid);
	}
	return grid;
}
// End debugging.

/* * * * * * * * * * * * * * * * * * * * * * * * * *
*					   QuadTree                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * */
QuadTree::QuadTree()
{
	tree_level = 0;
	sub_trees.resize(4);
	for (int i = 0; i < 4; i++)
		sub_trees[i] = new QuadTree(tree_level + 1);
}

QuadTree::QuadTree(int level)
{
	this->tree_level = level;
	sub_trees.resize(4);
	if (level == QuadTree::max_level - 1)
	{
		for (int i = 0; i < 4; i++)

			sub_trees[i] = nullptr;
		return;
	}
	for (int i = 0; i < 4; i++)
		sub_trees[i] = new QuadTree(tree_level + 1);
}

QuadTree::QuadTree(int level, sf::FloatRect boundary)
{
	tree_level = level;
	tree_boundary = boundary;
	for (QuadTree* s : sub_trees)
		s = nullptr;
}

QuadTree::~QuadTree()
{
	freeTree();
}

//Getters
std::vector<sf::FloatRect> QuadTree::getSubTreeBoundaries()
{
	std::vector<sf::FloatRect> sub_tree_bounds;
	float half_width = (this->tree_boundary.width / 2);
	float half_height = (this->tree_boundary.height / 2);
	float root_x = (this->tree_boundary.left);
	float root_y = (this->tree_boundary.top);
	sub_tree_bounds.resize(4);
	sub_tree_bounds[SUBTREE::UPPERLEFT] = sf::FloatRect(root_x, root_y, half_width, half_height);
	sub_tree_bounds[SUBTREE::UPPERRIGHT] = sf::FloatRect(root_x + half_width, root_y, half_width, half_height);
	sub_tree_bounds[SUBTREE::LOWERRIGHT] = sf::FloatRect(root_x + half_width, root_y + half_height, half_width, half_height);
	sub_tree_bounds[SUBTREE::LOWERLEFT] = sf::FloatRect(root_x, root_y + half_height, half_width, half_height);
	return sub_tree_bounds;
}

std::vector<Object*> QuadTree::getCollisionCandidates(Object* current)
{
	std::vector<Object*> candidates;

	// Can only be run from the actual root of the Quad Tree.
	if (this->tree_level != 0)
		return candidates;

	// Find pointer to the subtree of which 'current' is an Object.
	QuadTree* objectSubTree = this->retreiveSubtreeOf(this, current);

	// Insert all the objects within the sub trees to objectSubTree.
	this->collisionCandidateSearch(&candidates);
	return candidates;
}

//Setters
void QuadTree::setRootBoundaries(sf::FloatRect boundaries)
{
	if (tree_level == 0)
		tree_boundary = boundaries;
}

void QuadTree::setLevel(int level)
{
	this->tree_level = level;
}

void QuadTree::setBoundary(sf::FloatRect boundary)
{
	this->tree_boundary = boundary;
}

//Others
void QuadTree::clearTree()
{
	this->tree_objects.clear();
	for (int i = SUBTREE::UPPERLEFT; i < SUBTREE::LOWERLEFT + 1; i++)
	{
		if (this->sub_trees[i] != nullptr)
			this->sub_trees[i]->clearTree();
	}
	this->has_sub_trees = false;

	// Debugging.
	if (this->tree_level == 0)
		this->grid_lines->clear();
}

void QuadTree::splitTree()
{
	std::vector<sf::FloatRect> sub_tree_bounds = this->getSubTreeBoundaries();
	sub_trees[SUBTREE::UPPERLEFT]->setBoundary(sub_tree_bounds[SUBTREE::UPPERLEFT]);
	sub_trees[SUBTREE::UPPERRIGHT]->setBoundary(sub_tree_bounds[SUBTREE::UPPERRIGHT]);
	sub_trees[SUBTREE::LOWERRIGHT]->setBoundary(sub_tree_bounds[SUBTREE::LOWERRIGHT]);
	sub_trees[SUBTREE::LOWERLEFT]->setBoundary(sub_tree_bounds[SUBTREE::LOWERLEFT]);
	this->has_sub_trees = true;

	// Debugging.
	for (int i = 0; i < 4; i++)
		this->grid_lines->push_back(sub_tree_bounds[i]);
}

void QuadTree::insertObject(Object* object)
{
	int sub_index;
	if (this->has_sub_trees)
	{
		sub_index = this->objectIndex(object);
		if (sub_index != SUBTREE::ROOT)							//If object fits in one of this QuadTree's
		{														//subtrees, add object to that subtree.
			this->sub_trees[sub_index]->insertObject(object);			
			return;
		}
	}
	
	this->tree_objects.push_back(object);						//Adds object to this QuadTree.
	if (this->tree_objects.size() > QuadTree::max_objects &&	
		this->tree_level < QuadTree::max_level - 1)
	{
		if (!this->has_sub_trees)
		{
			this->splitTree();
		}
		for (int i = 0; i < this->tree_objects.size(); i++)
		{
			sub_index = this->objectIndex(this->tree_objects[i]);
			if (sub_index != SUBTREE::ROOT)
			{
				this->sub_trees[sub_index]->insertObject(this->tree_objects[i]);
				this->tree_objects.erase(this->tree_objects.begin() + i);
				i = 0;
			}
		}
	}
}

int QuadTree::objectIndex(Object* object)
{
	std::vector<sf::FloatRect> sub_tree_bounds = this->getSubTreeBoundaries();
	sf::Vector2f object_upperleft = object->getWorldPosition();				//Upper left corner of object.
	sf::Vector2f object_lowerright = object_upperleft + object->getSize();  //Lower right corner of object.
	for (int i = SUBTREE::UPPERLEFT; i < SUBTREE::LOWERLEFT; i++)
	{
		if (sub_tree_bounds[i].contains(object_upperleft) &&		//Checks if an object rectangle fits within any of the subtrees' boundaries. 
			sub_tree_bounds[i].contains(object_lowerright))			
			return i;
	}
	return (int)SUBTREE::ROOT;										//If the object doesn't fit within any of the subtrees
}																	//it belongs to the root of the subtrees (this QuadTree basically).

void QuadTree::freeTree()
{
	tree_objects.clear();
	this->has_sub_trees = false;
	for (int i = SUBTREE::UPPERLEFT; i < SUBTREE::LOWERLEFT + 1; i++)
	{
		if (sub_trees[i] != nullptr)
		{
			sub_trees[i]->freeTree();					//Recursively removes objects and frees Quad Tree memory.
			delete sub_trees[i];
			sub_trees[i] = nullptr;
		}
	}
}

// Debugging.
void QuadTree::setGridVector(std::vector<sf::FloatRect>* grid)
{
	if (this->tree_level < this->max_level - 1)
	{
		for (int i = 0; i < 4; i++)
			this->sub_trees[i]->setGridVector(grid);
	}
	this->grid_lines = grid;
}
// End debugging.

void QuadTree::collisionCandidateSearch(std::vector<Object*>* candidates)
{
	// Insert the objects in the current tree node.
	candidates->insert(candidates->end(), this->tree_objects.begin(), this->tree_objects.end());

	// Insert the objects of the sub tree nodes.
	if (this->has_sub_trees)
	{
		sub_trees[SUBTREE::UPPERLEFT]->collisionCandidateSearch(candidates);
		sub_trees[SUBTREE::UPPERRIGHT]->collisionCandidateSearch(candidates);
		sub_trees[SUBTREE::LOWERRIGHT]->collisionCandidateSearch(candidates);
		sub_trees[SUBTREE::LOWERLEFT]->collisionCandidateSearch(candidates);
	}
}

QuadTree* QuadTree::retreiveSubtreeOf(QuadTree* root, Object* o)
{
	int sub_index = this->objectIndex(o);
	if (sub_index != SUBTREE::ROOT && this->has_sub_trees)
		return this->sub_trees[sub_index]->retreiveSubtreeOf(this, o);
	return this;
}

/************************************************************************
**************************************************************************
*************************************************************************
************************************************************************
**********************************************************************
**************************************************************************
*************************************************************************
************************************************************************
**********************************************************************
**************************************************************************
*************************************************************************
************************************************************************
**********************************************************************
**************************************************************************
*************************************************************************
*************************************************************************/


/* * * * * * * * * * * * * * * * * * * * * * * * * *
*					   QuadTree                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * */
QuadTree_Hitbox::QuadTree_Hitbox()
{
	tree_level = 0;
	sub_trees.resize(4);
	for (int i = 0; i < 4; i++)
		sub_trees[i] = new QuadTree_Hitbox(tree_level + 1);
}

QuadTree_Hitbox::QuadTree_Hitbox(int level)
{
	this->tree_level = level;
	sub_trees.resize(4);
	if (level == QuadTree_Hitbox::max_level - 1)
	{
		for (int i = 0; i < 4; i++)

			sub_trees[i] = nullptr;
		return;
	}
	for (int i = 0; i < 4; i++)
		sub_trees[i] = new QuadTree_Hitbox(tree_level + 1);
}

QuadTree_Hitbox::QuadTree_Hitbox(int level, sf::FloatRect boundary)
{
	tree_level = level;
	tree_boundary = boundary;
	for (QuadTree_Hitbox* s : sub_trees)
		s = nullptr;
}

QuadTree_Hitbox::~QuadTree_Hitbox()
{
	freeTree();
}

//Getters
std::vector<sf::FloatRect> QuadTree_Hitbox::getSubTreeBoundaries()
{
	std::vector<sf::FloatRect> sub_tree_bounds;
	float half_width = (this->tree_boundary.width / 2);
	float half_height = (this->tree_boundary.height / 2);
	float root_x = (this->tree_boundary.left);
	float root_y = (this->tree_boundary.top);
	sub_tree_bounds.resize(4);
	sub_tree_bounds[SUBTREE::UPPERLEFT] = sf::FloatRect(root_x, root_y, half_width, half_height);
	sub_tree_bounds[SUBTREE::UPPERRIGHT] = sf::FloatRect(root_x + half_width, root_y, half_width, half_height);
	sub_tree_bounds[SUBTREE::LOWERRIGHT] = sf::FloatRect(root_x + half_width, root_y + half_height, half_width, half_height);
	sub_tree_bounds[SUBTREE::LOWERLEFT] = sf::FloatRect(root_x, root_y + half_height, half_width, half_height);
	return sub_tree_bounds;
}

std::vector<HitboxNode*> QuadTree_Hitbox::getCollisionCandidates(HitboxNode* current)
{
	std::vector<HitboxNode*> candidates;

	// Can only be run from the actual root of the Quad Tree.
	if (this->tree_level != 0)
		return candidates;

	// Find pointer to the subtree of which 'current' is an HitboxNode.
	QuadTree_Hitbox* HitboxNodeSubTree = this->retreiveSubtreeOf(this, current);

	// Insert all the HitboxNodes within the sub trees to HitboxNodeSubTree.
	this->collisionCandidateSearch(&candidates);
	return candidates;
}

//Setters
void QuadTree_Hitbox::setRootBoundaries(sf::FloatRect boundaries)
{
	if (tree_level == 0)
		tree_boundary = boundaries;
}

void QuadTree_Hitbox::setLevel(int level)
{
	this->tree_level = level;
}

void QuadTree_Hitbox::setBoundary(sf::FloatRect boundary)
{
	this->tree_boundary = boundary;
}

//Others
void QuadTree_Hitbox::clearTree()
{
	this->tree_objects.clear();
	for (int i = SUBTREE::UPPERLEFT; i < SUBTREE::LOWERLEFT + 1; i++)
	{
		if (this->sub_trees[i] != nullptr)
			this->sub_trees[i]->clearTree();
	}
	this->has_sub_trees = false;
}

void QuadTree_Hitbox::splitTree()
{
	std::vector<sf::FloatRect> sub_tree_bounds = this->getSubTreeBoundaries();
	sub_trees[SUBTREE::UPPERLEFT]->setBoundary(sub_tree_bounds[SUBTREE::UPPERLEFT]);
	sub_trees[SUBTREE::UPPERRIGHT]->setBoundary(sub_tree_bounds[SUBTREE::UPPERRIGHT]);
	sub_trees[SUBTREE::LOWERRIGHT]->setBoundary(sub_tree_bounds[SUBTREE::LOWERRIGHT]);
	sub_trees[SUBTREE::LOWERLEFT]->setBoundary(sub_tree_bounds[SUBTREE::LOWERLEFT]);
	this->has_sub_trees = true;
}

void QuadTree_Hitbox::insertObject(HitboxNode* object)
{
	int sub_index;
	if (this->has_sub_trees)
	{
		sub_index = this->objectIndex(object);
		if (sub_index != SUBTREE::ROOT)							//If object fits in one of this QuadTree_Hitbox's
		{														//subtrees, add object to that subtree.
			this->sub_trees[sub_index]->insertObject(object);
			return;
		}
	}

	this->tree_objects.push_back(object);						//Adds object to this QuadTree_Hitbox.
	if (this->tree_objects.size() > QuadTree_Hitbox::max_objects &&
		this->tree_level < QuadTree_Hitbox::max_level - 1)
	{
		if (!this->has_sub_trees)
		{
			this->splitTree();
		}
		for (int i = 0; i < this->tree_objects.size(); i++)
		{
			sub_index = this->objectIndex(this->tree_objects[i]);
			if (sub_index != SUBTREE::ROOT)
			{
				this->sub_trees[sub_index]->insertObject(this->tree_objects[i]);
				this->tree_objects.erase(this->tree_objects.begin() + i);
				i = 0;
			}
		}
	}
}

int QuadTree_Hitbox::objectIndex(HitboxNode* object)
{
	std::vector<sf::FloatRect> sub_tree_bounds = this->getSubTreeBoundaries();
	sf::FloatRect o_bb = object->getBB();
	sf::Vector2f object_upperleft = {o_bb.left, o_bb.top};												     //Upper left corner of object.
	sf::Vector2f object_lowerright = { object_upperleft.x + o_bb.width, object_upperleft.y + o_bb.height };  //Lower right corner of object.
	for (int i = SUBTREE::UPPERLEFT; i < SUBTREE::LOWERLEFT; i++)
	{
		if (sub_tree_bounds[i].contains(object_upperleft) &&		//Checks if an object rectangle fits within any of the subtrees' boundaries. 
			sub_tree_bounds[i].contains(object_lowerright))
			return i;
	}
	return (int)SUBTREE::ROOT;										//If the object doesn't fit within any of the subtrees
}																	//it belongs to the root of the subtrees (this QuadTree_Hitbox basically).

void QuadTree_Hitbox::freeTree()
{
	tree_objects.clear();
	this->has_sub_trees = false;
	for (int i = SUBTREE::UPPERLEFT; i < SUBTREE::LOWERLEFT + 1; i++)
	{
		if (sub_trees[i] != nullptr)
		{
			sub_trees[i]->freeTree();					//Recursively removes objects and frees Quad Tree memory.
			delete sub_trees[i];
			sub_trees[i] = nullptr;
		}
	}
}

// Debugging.
void QuadTree_Hitbox::setGridVector(std::vector<sf::FloatRect>* grid)
{
	if (this->tree_level < this->max_level - 1)
	{
		for (int i = 0; i < 4; i++)
			this->sub_trees[i]->setGridVector(grid);
	}
}
// End debugging.

void QuadTree_Hitbox::collisionCandidateSearch(std::vector<HitboxNode*>* candidates)
{
	// Insert the objects in the current tree node.
	candidates->insert(candidates->end(), this->tree_objects.begin(), this->tree_objects.end());

	// Insert the objects of the sub tree nodes.
	if (this->has_sub_trees)
	{
		sub_trees[SUBTREE::UPPERLEFT]->collisionCandidateSearch(candidates);
		sub_trees[SUBTREE::UPPERRIGHT]->collisionCandidateSearch(candidates);
		sub_trees[SUBTREE::LOWERRIGHT]->collisionCandidateSearch(candidates);
		sub_trees[SUBTREE::LOWERLEFT]->collisionCandidateSearch(candidates);
	}
}

QuadTree_Hitbox* QuadTree_Hitbox::retreiveSubtreeOf(QuadTree_Hitbox* root, HitboxNode* o)
{
	int sub_index = this->objectIndex(o);
	if (sub_index != SUBTREE::ROOT && this->has_sub_trees)
		return this->sub_trees[sub_index]->retreiveSubtreeOf(this, o);
	return this;
}