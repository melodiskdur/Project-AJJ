#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "PhysicsAttribute.h"
#include "Hitbox.h"
#include "CollisionDetection.h"
#include "CollisionGraph.h"
#include <math.h>
#include "Gravity.h"
#include "AirFriction.h"

// "etc" ...

class CollisionGraph;

/* PhysicsManager
* Handles all physics-related things.
*/
class PhysicsManager
{
public:
	PhysicsManager(std::vector<Object*>& objects);
	~PhysicsManager();

	void addAttribute(PhysicsAttribute* attribute);
	void basicCollisionHandler(sf::FloatRect view_rect);
	PhysicsAttribute* searchAttribute(std::string attribute_name);

	// DEBUGGING.
	CollisionGraph* col_graph;
	// END DEBUGGING.
private:
	std::vector<PhysicsAttribute*> attributes;
	std::vector<Object*>* scene_objects = nullptr;

	sf::Clock timer;
};