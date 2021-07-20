#pragma once
#include "Object.h"

class TestObject :
    public Object
{
public:
    TestObject(sf::Vector2f pos, sf::Vector2f size);
    ~TestObject();
private:
};

