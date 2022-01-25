#include "AirFriction.h"

AirFriction::AirFriction(std::vector<Object*>& objects)
	: PhysicsAttribute("Air Friction")
{
	this->scene_objects = &objects;
}

AirFriction::~AirFriction()
{

}

// Setters.
void AirFriction::setAirFriction(float fr)
{
	this->friction_constant = (fr >= this->limits.x && fr <= this->limits.y) ?
							fr : 5;
}


// Other.

/* Finds the objects within the window coordinates of a scene that contain 
* the "Air Friction" attribute and apples the friction to them, given that
* they are moving. */
void AirFriction::applyAirFriction(sf::FloatRect window_rect)
{
	this->friction_objects.clear();
	// Find all the objects within the view_rect of 
	// the window that contain the Gravity attribute.
	this->findFrictionObjects(window_rect);

	// Apply Gravity force on each object.
	for (int i = 0; i < this->friction_objects.size(); i++)
	{
		sf::Vector2f vel = this->friction_objects[i]->getVelocity();
		sf::Vector2f new_vel = vel;
		sf::Vector2f pos = this->friction_objects[i]->getWorldPosition();
		sf::Vector2f new_pos = pos;
		// Check if the current velocity of an object is bigger than the cutoff.
		new_vel.x = std::abs(vel.x) > this->cut_off ?
			(vel.x - this->friction_constant * (vel.x + 1/vel.x)) : 0.f;
		new_vel.y = std::abs(vel.y) > this->cut_off ?
			(vel.y - this->friction_constant * (this->friction_constant * vel.y + 1 /vel.y)) : 0.f;

		this->friction_objects[i]->setWorldPosition(pos + (new_vel - vel));
		this->friction_objects[i]->setVelocity(new_vel);
	}
}

/* Helper function to applyAirFriction. Iterates through the scene_objects
* vector to find the objects that contain the Air Friction attribute. */
void AirFriction::findFrictionObjects(sf::FloatRect window_rect)
{
	for (int i = 0; i < this->scene_objects->size(); i++)
	{
		for (std::string s : scene_objects->at(i)->getPhysicsAttributes())
		{
			if (s.compare("Air Friction") == 0)
			{
				sf::Vector2f pos = scene_objects->at(i)->getWorldPosition();
				sf::Vector2f vel = scene_objects->at(i)->getVelocity();
				// Upon finding a potential friction object, check if it's located within 
				// the window's view rectangle and if it has a non-zero velocity vector.
				if (pos.x >= window_rect.left && pos.x <= window_rect.left + window_rect.width &&
					pos.y >= window_rect.top && pos.y <= window_rect.top + window_rect.height &&
					(vel.x != 0 || vel.y != 0))
					this->friction_objects.push_back(scene_objects->at(i));
				break;
			}
		}
	}
}