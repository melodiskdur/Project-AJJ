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

	// TEMPORARY
	this->stacked_collisions.clear();
	// END TEMPORARY

	if (this->scene_objects == nullptr)
		return;

	// Gravity, Air Friction here.

	// Find Gravity attribute.
	Gravity* grav = (Gravity*)this->searchAttribute("Gravity");
	grav->applyGravity(view_rect);

	// Find Air Friction attribute.
	AirFriction* fric = (AirFriction*)this->searchAttribute("Air Friction");
	fric->applyAirFriction(view_rect);

	// Find Collision Detection attribute.
	CollisionDetection* col_det = (CollisionDetection*) this->searchAttribute("Collision Detection");
	if (col_det == nullptr)
		return;

	// Run Collision check and retreive the tuples of colliding objects.
	// NOTE: CollisionDetection should maybe be moved to a higher instance (collisions could be used for more
	// than just physics).
	std::vector<ObjectTuple> collision_tuples = col_det->getCollisions(view_rect);

	// Collect Hitbox-resolutions and add all the data to the CollisionData-vector.
	for (int i = 0; i < collision_tuples.size(); i++)
	{
		std::vector<ObjectData> current_data = Hitbox::separateHitboxes(collision_tuples[i].obj_i, collision_tuples[i].obj_j);
		for (int j = 0; j < current_data.size(); j++)
		{
			this->storeObjectData(current_data[j]);
		}
	}

	this->sortData();

	// Calculate the average Hitbox-resolutions for each Object.
	for (int i = 0; i < this->data.size(); i++)
	{
		this->setAverageHitboxRes(this->data[i]);
	}

	// Set all new positions.
	for (int i = 0; i < this->data.size(); i++)
	{
		this->data[i].m_object->setWorldPosition(this->data[i].m_final_repos);
		this->data[i].m_object->setVelocity(this->data[i].m_revelocities[data[i].indices[0]]);
	}

	// TEMPORARY
	this->postResCleanUp();
	// END TEMPORARY

	

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

void PhysicsManager::storeObjectData(ObjectData odata)
{
	// Check if the Object already has a CollisionData instance.
	int index = this->indexOf(odata.m_object);
	// If not: Append the CollisionData-vector.
	if (index == -1)
	{
		CollisionData new_data;
		new_data.m_object = odata.m_object;
		this->data.push_back(new_data);
		index = this->data.size() - 1;
	}
	// Add values.
	this->data[index].m_repositions.push_back(odata.m_wp);
	this->data[index].m_revelocities.push_back(odata.m_vel);
	this->data[index].m_reacc.push_back(odata.m_acc);
	this->data[index].m_colliding_objects.push_back(odata.m_colliding_object);
	this->data[index].m_hitboxes.push_back(odata.m_colliding_hitbox);
	this->data[index].m_intersects.push_back(odata.m_intersect);

	// TEMPORARY
	// Temporary functionality to handle stacked collisions.
	int i = this->findStack(this->data[index], odata.m_intersect);
	if (i != -1)
	{
		StackedCollision new_stack;
		new_stack.m_object = this->data[index].m_object;
		new_stack.m_collider1 = odata.m_colliding_object;
		new_stack.m_collider2 = this->data[index].m_colliding_objects[i];
		this->stacked_collisions.push_back(new_stack);
	}
	// END TEMPORARY
}

int PhysicsManager::indexOf(Object* o)
{
	for (int i = 0; i < this->data.size(); i++)
	{
		if (data[i].m_object == o)
			return i;
	}
	// -1 : Object doesn't exist in the Collision Data vector.
	return -1;
}

/* Calculates the new world position of object 'data.m_object' from the collision data
*  gathered using the Hitbox collision resolves (see Hitbox.h/cpp).
*/ 
void PhysicsManager::setAverageHitboxRes(CollisionData& data)
{
	Object* o = data.m_object;
	sf::Vector2f opos = o->getWorldPosition();
	sf::Vector2f osize = o->getSize();
	// Vector where each element corresponds to a collision. 0: Unresolved. 1: Resolved.
	std::vector<int> unresolved;
	unresolved.resize(data.m_colliding_objects.size());
	std::fill(unresolved.begin(), unresolved.end(), 0);
	int num_of_unresolved = unresolved.size();

	// The positions that will be assigned to the object at the end of the function.
	std::vector<sf::Vector2f> repos_vector;
	sf::Vector2f final_repos;

	// Parameters to be used in the loop.
	sf::FloatRect best_hitbox;
	sf::FloatRect current_hitbox;
	std::vector<int> best_resolved_indices;
	std::vector<int> current_resolved_indices;
	int best_resolved;
	int current_resolved;
	int best_index;
	int current_index;
	int iterations = 0;
	int blabla = num_of_unresolved;

	// While there are still unresolved collisions.
	while (num_of_unresolved > 0)
	{
		// To avoid infinite loops. If the function can't find a decent resolve,
		// we'll return the current world position of the object as a last resort.
		if (iterations++ > blabla)
		{
			repos_vector.push_back(opos);
			data.indices.push_back(0);
			break;
		}

		best_index = 0;
		best_resolved = 0;
		for (int i = 0; i < unresolved.size(); i++)
		{
			// If already resolved: skip to next.
			if (unresolved[i] == 1) continue;
			// Set up a temporary resolved hitbox for the object.
			current_hitbox = sf::FloatRect(data.m_repositions[i], osize);
			// Clear the current resolved indices-vector.
			current_resolved_indices.clear();
			// Set the number of resolves for this resolved hitbox to 0.
			current_resolved = 0;
			current_index = i;
			for (int j = 0; j < unresolved.size(); j++)
			{
				// If already resolved: skip to next.
				if (unresolved[j] == 1) continue;

				// Check if a collision is resolved. Increment current_resolved if true and at the index
				// of the solved collision.
				if (!CollisionDetection::areIntersecting(current_hitbox, data.m_hitboxes[j]))
				{
					current_resolved++;
					current_resolved_indices.push_back(j);
				}
			}
			// If we've found a better resolve than the best: update the best.
			if (current_resolved > best_resolved)
			{
				best_resolved = current_resolved;
				best_resolved_indices = current_resolved_indices;
				best_index = current_index;
				best_hitbox = current_hitbox;
			}
		}
		// Resolve the collisions.
		if (best_resolved == 0) continue;
		for (int i = 0; i < best_resolved_indices.size(); i++)
		{
			unresolved[best_resolved_indices[i]] = 1;
		}
		num_of_unresolved -= best_resolved;
		repos_vector.push_back(sf::Vector2f(best_hitbox.left, best_hitbox.top));
		data.indices.push_back(best_index);
	}
	// Finally, update the position of the object.
	final_repos = repos_vector[0];
	for (int i = 1; i < repos_vector.size(); i++)
	{
		sf::Vector2f dpos = repos_vector[i] - opos;
		final_repos += dpos;
	}
	data.m_final_repos = final_repos;
}

void PhysicsManager::sortData()
{
	if (this->data.size() == 0) return;
	CollisionData current;
	for (int i = 0; i < this->data.size() - 1; i++)
	{
		current = data[i];
		for (int j = i + 1; j < this->data.size(); j++)
		{
			if (current.m_colliding_objects.size() < data[j].m_colliding_objects.size())
			{
				data[i] = data[j];
				data[j] = current;
				break;
			}
		}
	}
}

int PhysicsManager::findStack(CollisionData& data, INTERSECTED_SIDE side)
{
	for (int i = 0; i < data.m_intersects.size(); i++)
	{
		INTERSECTED_SIDE current = data.m_intersects[i];
		if ((int) side % 2 == (int) current % 2 && side != current) return i;
	}
	return -1;
}

void PhysicsManager::postResCleanUp()
{
	Object* current,* stacked1,* stacked2;
	sf::FloatRect cur_hitbox, stack1_hitbox, stack2_hitbox;
	std::vector<sf::Vector2f> resolves;

	for (int i = 0; i < this->stacked_collisions.size(); i++)
	{
		current = this->stacked_collisions[i].m_object;
		stacked1 = this->stacked_collisions[i].m_collider1;
		stacked2 = this->stacked_collisions[i].m_collider2;

		cur_hitbox = { current->getWorldPosition(), current->getSize() };
		stack1_hitbox = { stacked1->getWorldPosition(), stacked1->getSize() };
		stack2_hitbox = { stacked2->getWorldPosition(), stacked2->getSize() };
		// Check if current is still intersecting with any of the objects.
		if (CollisionDetection::areIntersecting(cur_hitbox, stack1_hitbox))
		{
			resolves = Hitbox::unstuck(stack1_hitbox, cur_hitbox, stacked1->getVelocity());
			stacked1->setWorldPosition(resolves[0]);
			stacked1->setVelocity(resolves[1]);
		}
		else if (CollisionDetection::areIntersecting(cur_hitbox, stack2_hitbox))
		{
			resolves = Hitbox::unstuck(stack2_hitbox, cur_hitbox, stacked2->getVelocity());
			stacked2->setWorldPosition(resolves[0]);
			stacked2->setVelocity(resolves[1]);
		}
	}
}
