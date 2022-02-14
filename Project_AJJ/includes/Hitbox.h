#pragma once
#include "PhysicsAttribute.h"
#include "Object.h"
#include <math.h>

class Hitbox :
    public PhysicsAttribute
{
public: 
    Hitbox();
    ~Hitbox();

    //Getters

    //Setters

    //Others
    // Main function for separation. Calls the object seperation functions
    // below depending on if only one or both objects are moving.
    static std::vector<ObjectData> separateHitboxes(Object* i, Object* j);
    // Resolves collisions with only one moving object.
    static ObjectData singleObjectSeparation(Object* moving, Object* other);
    static ObjectData singleObjectSeparation2(Object* moving, Object* other);
    // Resolves collisions where both objects are moving.
    static std::vector<ObjectData> dualObjectSeparation(Object*i, Object* j);

    // "Last resort" function that unstucks (separates) the hitboxes of
    // two objects (one moving, one still). Moves the moving.hitbox
    // the shortest distance (along x or y) that resolves the collision.
    static std::vector<sf::Vector2f> unstuck(sf::FloatRect moving, sf::FloatRect other, sf::Vector2f moving_vel);
    static sf::Vector2f unstuck(sf::FloatRect moving, sf::FloatRect other);
    
    // NOTE: DESCRIPTION HERE.
    static std::vector<sf::Vector2f> recalibrate(const ObjectData& odata, const sf::Vector2f& i_pos, const INTERSECTED_SIDE& adj_intersect, const sf::Vector2f& j_pos);
private:
    // Returns how much the two objects are overlapping each other,
    // along both the x- and the y-axis. (Used in dualObjectSeparation).
    static sf::Vector2f getOverlaps(Object* i, Object* j);
    static sf::Vector2f getOverlaps(sf::FloatRect i_rect, sf::FloatRect j_rect);

    // Simple checks to see if the two objects are travelling in parallell.
    static bool sameXDirection(Object* i, Object* j);
    static bool sameYDirection(Object* i, Object* j);
    // Calculates and returns the factors t_x and t_y that the object
    // x- and y-velocities must be multiplied by to undo the x- and y-
    // overlaps. (Used in dualObjectSeparation).
    static sf::Vector2f getT(Object* i, Object* j, sf::Vector2f overlaps);
};

