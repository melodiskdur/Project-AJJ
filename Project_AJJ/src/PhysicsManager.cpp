#include "PhysicsManager.h"

unsigned int PhysicsManager::instance_counter = 0;

PhysicsManager::PhysicsManager() { PhysicsManager::instance_counter++; }

PhysicsManager::PhysicsManager(std::vector<Object*>& objects) : PhysicsManager()
{
	this->scene_objects = &objects;
}

PhysicsManager::~PhysicsManager() { PhysicsManager::instance_counter--; }

void PhysicsManager::addAttribute(PhysicsAttribute* attribute)
{
	attributes.push_back(attribute);
}

void PhysicsManager::basicCollisionHandler(sf::FloatRect view_rect)
{
	if (this->scene_objects == nullptr)
		return;

	// DEBUGGING.
	sf::FloatRect extra_view_rect = { sf::Vector2f(view_rect.left - 1000.f, view_rect.top - 1000.f), 10.0f * sf::Vector2f( view_rect.width, view_rect.height)};
	// END DEBUGGING.

	// Gravity, Air Friction here.

	// Find Gravity attribute.
	Gravity* grav = (Gravity*)this->searchAttribute("Gravity");
	grav->applyGravity(extra_view_rect);

	// Find Air Friction attribute.
    AirFriction* fric = (AirFriction*)this->searchAttribute("Air Friction");
	fric->applyAirFriction(extra_view_rect);

	// Find Collision Detection attribute.
	CollisionDetection* col_det = (CollisionDetection*) this->searchAttribute("Collision Detection");
	if (col_det == nullptr)
		return;

	// Run Collision check and retreive the tuples of colliding objects as well as close calls to each object.
	// NOTE: CollisionDetection should maybe be moved to a higher instance (collisions could be used for more
	// than just physics).
	std::vector<ObjectTuple> collision_tuples = col_det->getCollisions(extra_view_rect);
	std::vector<CloseCallHolder> close_calls = col_det->getCloseCallHolders();
	
	// Load all collisions and close calls into the Physics Manager's collision graph. It stores each
	// Object in a CollisionNode and the collision between two Objects as a CollisionEdge between the
	// corresponding nodes. Finally, relax the graph.
	this->col_graph->storeCollisions(collision_tuples);
	this->col_graph->storeCloseCalls(close_calls);
	this->col_graph->resolveTree();
	this->col_graph->applyTreeResolution();

	// Momentum here.
}

PhysicsAttribute* PhysicsManager::searchAttribute(std::string attribute_name)
{
	for (PhysicsAttribute* pa : this->attributes)
	{
		if (pa->getId().compare(attribute_name) == 0)
			return pa;
	}
	return nullptr;
}