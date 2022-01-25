#include "Gravity.h"

Gravity::Gravity(std::vector<Object*>& scene_ob):
	PhysicsAttribute("Gravity")
{
	this->scene_objects = &scene_ob;
}

Gravity::~Gravity()
{

}

// Getters.
sf::Vector2f Gravity::getGravityFactors() { return this->gravity_factors; }

// Setters.
void Gravity::setGravityFactors(sf::Vector2f g_f)
{
	this->gravity_factors = g_f;
}

void Gravity::setGravityXFactor(float g_x)
{
	this->gravity_factors.y = g_x;
}

void Gravity::setGravityYFactor(float g_y)
{
	this->gravity_factors.y = g_y;
}

// Other.

/* Applies the gravity factors the the velocities of 
* the objects within the visible part of the scene 
* (within the window). 
*/
void Gravity::applyGravity(sf::FloatRect window_rect)
{
	this->gravity_objects.clear();
	// Find all the objects within the view_rect of 
	// the window that contain the Gravity attribute.
	this->findGravityObjects(window_rect);

	// Apply Gravity force on each object.
	for (int i = 0; i < this->gravity_objects.size(); i++)
	{
		sf::Vector2f pos = this->gravity_objects[i]->getWorldPosition();
		sf::Vector2f vel = this->gravity_objects[i]->getVelocity();
		sf::Vector2f new_vel = vel + this->gravity_factors;
		sf::Vector2f new_pos = pos + (new_vel - vel);
		this->gravity_objects[i]->setWorldPosition(new_pos);
		this->gravity_objects[i]->setVelocity(new_vel);
	}
}

/* Helper function to applyGravity: Selects the objects
* within the window rect that have the Gravity attribute assigned
* to them.
*/
void Gravity::findGravityObjects(sf::FloatRect window_rect)
{
	for (int i = 0; i < this->scene_objects->size(); i++)
	{
		for (std::string s : scene_objects->at(i)->getPhysicsAttributes())
		{
			if (s.compare("Gravity") == 0)
			{
				sf::Vector2f pos = scene_objects->at(i)->getWorldPosition();
				// Upon finding a potential friction object, check if it's located within 
				// the window's view rectangle and if it has a non-zero velocity vector.
				if (pos.x >= window_rect.left && pos.x <= window_rect.left + window_rect.width &&
					pos.y >= window_rect.top && pos.y <= window_rect.top + window_rect.height)
					this->gravity_objects.push_back(scene_objects->at(i));
				break;
			}
		}
	}
}