#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Scene.h"
#include "PhysicsAttribute.h"
#include <cmath>

class QuadTree;

/* CollisionDetection
*  Subclass of PhysicsAttribute. Once added to a Scene instance, it can be used
   to handle the collision detection between the objects in that Scene.
*/
class CollisionDetection : public PhysicsAttribute
{
public:
	//Constructor that takes a pointer to a Scene's object vector as parameter.
	CollisionDetection(std::vector<Object*>& objects);
	~CollisionDetection();

	//Runs collisition detection for all objects within the view_rect boundaries.
	//The view_rect should be the Scene::camera_view_rect. 
	void checkForCollisions(sf::FloatRect view_rect);
private:
	std::vector<Object*>* scene_objects = nullptr;
	
	QuadTree* root = nullptr;								//The base of the QuadTree.
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
	QuadTree(int level, sf::FloatRect boundary);
	~QuadTree();

	//Getters
	//Helper function to split and return a node's region into four.
	std::vector<sf::FloatRect> getSubTreeBoundaries();
	//Calculates and returns a vector of objects that are within the
	//the same region of the Object* current, i.e potential collisions.
	std::vector<Object*> getCollisionCandidates(Object* current);

	//Setters
	//The base rectangle from which the Quad Tree recursion starts from.
	//Should correspond to the parameter view_rect in CollisionDetection::checkForCollisions().
	void setRootBoundaries(sf::FloatRect boundaries);

	//Others
	//
	void clearTree();
	//Splits a node into 4 sub trees.
	void splitTree();
	//Inserts the object into the Quad Tree, placing it in the correct node / sub tree.
	void insertObject(Object* object);
	//Calculates and returns which sub tree of a node that an object belongs to.
	int objectIndex(Object* object);

private:
	int tree_level;							//The level of which this node / sub tree is at.
	const static int max_level = 5;			//Tree can't go deeper than this.
	const static int max_objects = 10;		//A single node can't contain more objects than this.
	sf::FloatRect tree_boundary;
	QuadTree* sub_trees[4];
	bool has_sub_trees = false;
	std::vector<Object*> tree_objects;
};