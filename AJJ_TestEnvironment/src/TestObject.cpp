#include "../includes/TestObject.h"

TestObject::TestObject(sf::Vector2f pos, sf::Vector2f size)
	: Object(pos, size)
{
	std::cout << "TestObject called\n";
}

TestObject::~TestObject()
{
}