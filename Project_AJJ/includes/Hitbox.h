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
    static void separateHitboxes(Object* i, Object* j);
    static void singleObjectSeparation(Object* moving, Object* other);
    static void dualObjectSeparation(Object*i, Object* j);

private:
    //enum SIDE_COLLISION {left_side, right_side, upper_side, lower_side};

};

