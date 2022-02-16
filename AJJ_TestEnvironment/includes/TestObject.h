#pragma once
#include "Object.h"
#include "Action.h"
#include "Animation.h"

class TestObject :
    public Object
{
public:
    TestObject(sf::Vector2f pos, sf::Vector2f size);
    TestObject(sf::Vector2f pos, sf::Vector2f size, bool npc);
    ~TestObject();
private:
};

