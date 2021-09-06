#include "CollisionDetection.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * *
*					CollisionDetection             *
 * * * * * * * * * * * * * * * * * * * * * * * * * */
CollisionDetection::CollisionDetection(std::vector<Object*>& objects):
	PhysicsAttribute("Collision Detection")
{
	scene_objects = &objects;
	root = new QuadTree();
;}

CollisionDetection::~CollisionDetection()
{
	delete root;
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
			if ( (object_i.intersects(object_j) || object_j.intersects(object_i)) &&												//Intersection.
				rendered_objects.at(i) != rendered_objects.at(j) &&																	//Not the same object.
				!this->tupleExists(rendered_objects.at(i), rendered_objects.at(j)) )													//Collision is not already registered in vector.
			{
				//std::cout << i << " collides with " << j << "\n";
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

void CollisionDetection::handleCollisions()
{
	for (int i = 0; i < this->collisions.size(); i++)
	{
		//Collision handling
		Hitbox::separateHitboxes(collisions[i].obj_i, collisions[i].obj_j);
	}
}

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
	int half_width = (int) (this->tree_boundary.width / 2);
	int half_height = (int) (this->tree_boundary.height / 2);
	int root_x = static_cast<int>(this->tree_boundary.left);
	int root_y = static_cast<int>(this->tree_boundary.top);
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
	int sub_index = objectIndex(current);					//Get the sub tree that "current" belongs to. Returns SUBTREE:ROOT if it belongs to this node.
	if (has_sub_trees && sub_index != SUBTREE::ROOT)		
	{	//If "current" belongs to one of the node's sub trees. Retreives and returns the objects of the sub tree that "current" belongs to.
		std::vector<Object*> sub_candidates = sub_trees[sub_index]->getCollisionCandidates(current);
		candidates.insert(candidates.end(), sub_candidates.begin(), sub_candidates.end());
		return sub_candidates;
	}
	//If "current" does NOT belong to any of the node's sub trees. Retreives and returns the objects of this node (sub tree objects will be included).
	candidates.insert(candidates.end(), tree_objects.begin(), tree_objects.end());
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
}

void QuadTree::splitTree()
{
	std::vector<sf::FloatRect> sub_tree_bounds = this->getSubTreeBoundaries();
	sub_trees[SUBTREE::UPPERLEFT]->setBoundary(sub_tree_bounds[SUBTREE::UPPERLEFT]);
	sub_trees[SUBTREE::UPPERRIGHT]->setBoundary(sub_tree_bounds[SUBTREE::UPPERRIGHT]);
	sub_trees[SUBTREE::LOWERRIGHT]->setBoundary(sub_tree_bounds[SUBTREE::LOWERRIGHT]);
	sub_trees[SUBTREE::LOWERLEFT]->setBoundary(sub_tree_bounds[SUBTREE::LOWERLEFT]);
	this->has_sub_trees = true;
}

void QuadTree::insertObject(Object* object)
{
	int sub_index;
	if (has_sub_trees)
	{
		sub_index = objectIndex(object);
		if (sub_index != SUBTREE::ROOT)							//If object fits in one of this QuadTree's
		{														//subtrees, add object to that subtree.
			sub_trees[sub_index]->insertObject(object);			
			return;
		}
	}
	this->tree_objects.push_back(object);						//Adds object to this QuadTree.
	if (tree_objects.size() > QuadTree::max_objects &&	
		tree_level < QuadTree::max_level)
	{
		if (!has_sub_trees)
			splitTree();
		for (int i = 0; i < tree_objects.size(); i++)
		{
			sub_index = objectIndex(tree_objects[i]);
			if (sub_index != SUBTREE::ROOT)
			{
				sub_trees[sub_index]->insertObject(tree_objects[i]);
				tree_objects.erase(tree_objects.begin() + i);
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