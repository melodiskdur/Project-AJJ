#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "Scene.h"
#include "PhysicsAttribute.h"
#include <cmath>

class QuadTree;

/* CollisionDetection
*  Self-explanatory (WIP).
*/
class CollisionDetection : public PhysicsAttribute
{
public:
	CollisionDetection(std::vector<Object*>& objects);
	~CollisionDetection();

	//void test();
	//Runs collisition detection fr all objects within the view_rect boundaries.
	//The view_rect should be the Scene::camera_view_rect. 
	void checkForCollisions(sf::FloatRect view_rect);
private:
	std::vector<Object*>* scene_objects = nullptr;
	
	QuadTree* root = nullptr;
};

/* QuadTree
*  
*/
class QuadTree
{
	enum SUBTREE {UPPERLEFT = 0, UPPERRIGHT, LOWERRIGHT, LOWERLEFT, ROOT = -1};

public:
	QuadTree();
	QuadTree(int level, sf::FloatRect boundary);
	~QuadTree();

	//Getters
	std::vector<sf::FloatRect> getSubTreeBoundaries();
	std::vector<Object*> getCollisionCandidates(Object* current);

	//Setters
	//The base rectangle from which the Quad Tree recursion starts from.
	//Should correspond to the parameter view_rect in CollisionDetection::checkForCollisions();
	void setRootBoundaries(sf::FloatRect boundaries);

	//Others
	void clearTree();
	void splitTree();
	void insertObject(Object* object);
	int objectIndex(Object* object);

private:
	int tree_level;
	const static int max_level = 5;			//Tree can't go deeper than this.
	const static int max_objects = 10;		//A single QuadTree can't contain more than 10 objects.
	sf::FloatRect tree_boundary;
	QuadTree* sub_trees[4];
	bool has_sub_trees = false;
	std::vector<Object*> tree_objects;
};