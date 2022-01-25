#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"
#include "PhysicsAttribute.h"
#include <math.h>
#include "PhysicsAttribute.h"

/* Gravity
* Class that implements Gravitational force functionality onto Scene objects
* that contain the Gravity attribute.
*/
class Gravity :
    public PhysicsAttribute
{
public:
    Gravity(std::vector<Object*>& scene_ob);
    ~Gravity();

    // Getters.
    sf::Vector2f getGravityFactors();

    // Setters.
    void setGravityFactors(sf::Vector2f g_f);
    void setGravityXFactor(float g_x);
    void setGravityYFactor(float g_y);

    // Other.
    void applyGravity(sf::FloatRect window_rect);

private:
    sf::Vector2f gravity_factors = {0, 1};        // Gravitational force along x- and y-axis.
    std::vector<Object*>* scene_objects = nullptr;  // The objects of the current Scene.
    std::vector<Object*> gravity_objects;           // The objects that contain the Gravity attribute. 

    void findGravityObjects(sf::FloatRect window_rect);

};

