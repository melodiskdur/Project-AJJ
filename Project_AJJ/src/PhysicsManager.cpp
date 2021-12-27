#include "PhysicsManager.h"

PhysicsManager::PhysicsManager(std::vector<Object*>& objects)
{
	this->scene_objects = &objects;
}

PhysicsManager::~PhysicsManager()
{

}

void PhysicsManager::addAttribute(PhysicsAttribute* attribute)
{
	attributes.push_back(attribute);
}

void PhysicsManager::basicCollisionHandler(sf::FloatRect view_rect)
{
	this->data.clear();

	if (this->scene_objects == nullptr)
		return;
	
	// Find Collision Detection attribute.
	CollisionDetection* col_det = (CollisionDetection*) this->searchAttribute("Collision Detection");
	if (col_det == nullptr)
		return;

	// Run Collision check and retreive the tuples of colliding objects.
	std::vector<ObjectTuple> collision_tuples = col_det->getCollisions(view_rect);

}

PhysicsAttribute* PhysicsManager::searchAttribute(std::string attribute_name)
{
	for (PhysicsAttribute* pa : this->attributes)
	{
		if (pa->getId().compare(attribute_name) == 0)
			return pa;
	}
}