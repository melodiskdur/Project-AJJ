#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Object.h"
#include "Scene.h"
#include "HitboxNode.h"
#include "PhysicsAttribute.h"
#include "CollisionHandler.h"
#include <algorithm>
#include <cmath>

class QuadTree;
class QuadTree_Hitbox;
class HitboxNode;

/* Datatype to store all the close calls to a given object.
* Close calls are deemed to be the objects close to m_object,
* within the QuadTree, that are not intersecting with m_object.
* See CollisionDetetcion:getCollisions() where this datatype is
* used. */
typedef struct _CloseCallHolder
{
	Object* m_object = nullptr;
	std::vector<Object*> m_close_calls;
} CloseCallHolder;

typedef struct _CCHolder
{
	HitboxNode* m_n = nullptr;
	std::vector<HitboxNode*> m_cc;
} CCHolder;

/* Datatype to represent two objects in collision with each other.
*/
typedef struct _ObjectTuple
{
	Object* obj_i = nullptr;
	Object* obj_j = nullptr;
} ObjectTuple;

typedef struct _CollTuple
{
	HitboxNode* m_ni = nullptr;
	HitboxNode* m_nj = nullptr;
} CollTuple;

/* CollisionDetection
*  Subclass of PhysicsAttribute. Once added to a Scene instance, it can be used
*  to handle the collision detection between the objects in that Scene.
*
*  The actual collision detection is then done between an object and all the objects
*  in the same node or any sub trees of that node. 
*/
class CollisionDetection : public PhysicsAttribute
{
public:
	//Constructor that takes a pointer to a Scene's object vector as parameter.
	CollisionDetection(std::vector<Object*>& objects);
	~CollisionDetection();
	std::vector<CloseCallHolder> getCloseCallHolders() { return this->holders; };
	std::vector<CCHolder> getCCHolders() { return this->cc_holders; };
	//Runs collision detection for all objects within the view_rect boundaries.
	//The view_rect should be the Scene::camera_view_rect. Different versions below.
	std::vector<ObjectTuple> getCollisions(sf::FloatRect view_rect);
	std::vector<CollTuple> getCollisions_CT(sf::FloatRect view_rect);
	void checkForCollisions(sf::FloatRect view_rect);
	//Checks if tuple of colliding objects already exists within collisions vector.
	bool tupleExists(Object* o_i, Object* o_j);
	bool tupleExistsX(HitboxNode* n_i, HitboxNode* n_j);
	void handleCollisions();
	// Returns true if a tuple of collision candidates has at least one moving object.
	bool atLeastOneMoving(Object* i, Object* j);
	// Returns true if two objects have intersecting boundaries.
	static bool areIntersecting(sf::FloatRect ibox, sf::FloatRect jbox);
	// Checks if an object j is within a proximity of i->getSize() of object i.
	static bool isCloseCall(Object* i, Object* j);
	static bool isCloseCallX(HitboxNode* n_i, HitboxNode* n_j);
	// Checks and removes collisions that have been resolved.
	void removeResolved();

	void findHitboxes();

	// DEBUGGING.
	std::vector<sf::VertexArray> getGrid();
	std::vector<HitboxNode*> getHitboxes() { return this->hitboxes; };
	// END DEBUGGING.
private:
	std::vector<HitboxNode*> hitboxes;
	std::vector<CollTuple> coll_tuples;
	std::vector<CCHolder> cc_holders;
	std::vector<Object*>* scene_objects = nullptr;
	std::vector<ObjectTuple> collisions;
	std::vector<CloseCallHolder> holders;					// Close calls to each rendered object.
	QuadTree* root = nullptr;								// The base of the QuadTree.

	QuadTree_Hitbox* hb_root = nullptr;

	// Debugging.
	std::vector<sf::FloatRect> grid_lines;
};

/* QuadTree
*  A data structure that helps with the collision detection. Essentially
*  it is a Binary Tree but where each node contains 4 children instead of 2.
*  It splits the scene recursively into four regions (sub trees/nodes) and places 
*  the Scene objects into the region that contains each object's world position.
* 
*  The Quad Tree can hold a number of levels corresponding to the variable max_levels.
*/ 
class QuadTree
{
	//Enum that is used to help keep track of which sub tree of a node is being accessed.
	//For example sub_trees[UPPERLEFT] instead of sub_trees[0].
	enum SUBTREE {UPPERLEFT = 0, UPPERRIGHT, LOWERRIGHT, LOWERLEFT, ROOT = -1};

public:
	QuadTree();
	QuadTree(int level);
	QuadTree(int level, sf::FloatRect boundary);
	~QuadTree();

	//Getters
	//Helper function to split and return a node's region into four.
	std::vector<sf::FloatRect> getSubTreeBoundaries();
	// Calculates and returns a vector of objects that are within the
	// the same region of the Object* current, i.e potential collisions.
	// Can only be run from the ACTUAL root of the Quad Tree.
	std::vector<Object*> getCollisionCandidates(Object* current);

	//Setters
	//The base rectangle from which the Quad Tree recursion starts from.
	//Should correspond to the parameter view_rect in CollisionDetection::checkForCollisions().
	void setRootBoundaries(sf::FloatRect boundaries);
	void setLevel(int level);
	void setBoundary(sf::FloatRect boundary);

	//Others
	//Resets all the nodes in the QuadTree.
	void clearTree();
	//Splits a node into 4 sub trees.
	void splitTree();
	//Inserts the object into the Quad Tree, placing it in the correct node / sub tree.
	void insertObject(Object* object);
	//Calculates and returns which sub tree of a node that an object belongs to.
	int objectIndex(Object* object);
	//Called in ~QuadTree to free up memory of all nodes.
	void freeTree();

	// Debugging.
	void setGridVector(std::vector<sf::FloatRect>* grid);

private:
	int tree_level;							//The level of which this node / sub tree is at.
	const static int max_level = 5;			//Tree can't go deeper than this.
	const static int max_objects = 10;		//A single node can't contain more objects than this.
	sf::FloatRect tree_boundary;
	std::vector<QuadTree*> sub_trees;
	bool has_sub_trees = false;
	std::vector<Object*> tree_objects;

	// Function used inside getCollisionCandidates(). Recursively adds all
	// sub-tree objects of a QuadTree node to a pointer to an Object vector.
	void collisionCandidateSearch(std::vector<Object*>* candidates);
	// Function to retreive a pointer of a subtree which contains a given Object.
	QuadTree* retreiveSubtreeOf(QuadTree* root, Object* o);

	// Debugging
	std::vector<sf::FloatRect>* grid_lines = nullptr;
};





/* QuadTree
*  A data structure that helps with the collision detection. Essentially
*  it is a Binary Tree but where each node contains 4 children instead of 2.
*  It splits the scene recursively into four regions (sub trees/nodes) and places
*  the Scene objects into the region that contains each object's world position.
*
*  The Quad Tree can hold a number of levels corresponding to the variable max_levels.
*/
class QuadTree_Hitbox
{
	//Enum that is used to help keep track of which sub tree of a node is being accessed.
	//For example sub_trees[UPPERLEFT] instead of sub_trees[0].
	enum SUBTREE { UPPERLEFT = 0, UPPERRIGHT, LOWERRIGHT, LOWERLEFT, ROOT = -1 };

public:
	QuadTree_Hitbox();
	QuadTree_Hitbox(int level);
	QuadTree_Hitbox(int level, sf::FloatRect boundary);
	~QuadTree_Hitbox();

	//Getters
	//Helper function to split and return a node's region into four.
	std::vector<sf::FloatRect> getSubTreeBoundaries();
	// Calculates and returns a vector of objects that are within the
	// the same region of the Object* current, i.e potential collisions.
	// Can only be run from the ACTUAL root of the Quad Tree.
	std::vector<HitboxNode*> getCollisionCandidates(HitboxNode* current);

	//Setters
	//The base rectangle from which the Quad Tree recursion starts from.
	//Should correspond to the parameter view_rect in CollisionDetection::checkForCollisions().
	void setRootBoundaries(sf::FloatRect boundaries);
	void setLevel(int level);
	void setBoundary(sf::FloatRect boundary);

	//Others
	//Resets all the nodes in the QuadTree.
	void clearTree();
	//Splits a node into 4 sub trees.
	void splitTree();
	//Inserts the object into the Quad Tree, placing it in the correct node / sub tree.
	void insertObject(HitboxNode* object);
	//Calculates and returns which sub tree of a node that an object belongs to.
	int objectIndex(HitboxNode* object);
	//Called in ~QuadTree to free up memory of all nodes.
	void freeTree();

	// Debugging.
	void setGridVector(std::vector<sf::FloatRect>* grid);

private:
	int tree_level;							//The level of which this node / sub tree is at.
	const static int max_level = 5;			//Tree can't go deeper than this.
	const static int max_objects = 10;		//A single node can't contain more objects than this.
	sf::FloatRect tree_boundary;
	std::vector<QuadTree_Hitbox*> sub_trees;
	bool has_sub_trees = false;
	std::vector<HitboxNode*> tree_objects;

	// Function used inside getCollisionCandidates(). Recursively adds all
	// sub-tree objects of a QuadTree node to a pointer to an Object vector.
	void collisionCandidateSearch(std::vector<HitboxNode*>* candidates);
	// Function to retreive a pointer of a subtree which contains a given Object.
	QuadTree_Hitbox* retreiveSubtreeOf(QuadTree_Hitbox* root, HitboxNode* n);
};