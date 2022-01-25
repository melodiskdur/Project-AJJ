#pragma once
#include "PhysicsAttribute.h"

/* AirFriction
* PhysicsAttribute type class that implements a simple air friction 
* counter force onto moving objects that contain the attribute.
* NOTE: Friction related to objects colliding (for example a
* character gliding on ice blocks...) will be in another class.
*/
class AirFriction :
    public PhysicsAttribute
{
public:
    AirFriction(std::vector<Object*>& objects);
    ~AirFriction();

    // Getters.
    float getAirFriction() { return friction_constant; };

    // Setters.
    void setAirFriction(float fr);

    // Other.
    void applyAirFriction(sf::FloatRect window_rect);
private:
    float friction_constant = 0.05;              // Should be between the x and y values of limits.          
    sf::Vector2f limits = { 0.f, 1.f};          // See above.
    float cut_off = 0.4;                        // Cut-off value for when the velocity of an object should be set to 1.
    std::vector<Object*>* scene_objects;        // Objects connected to the current scene.
    std::vector<Object*> friction_objects;      // Objects connected to the current scene with the Air Friction attribute.

    void findFrictionObjects(sf::FloatRect window_rect);
};

