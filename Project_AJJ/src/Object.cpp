#include "Object.h"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <iostream>

Object::Object(sf::Vector2f pos, sf::Vector2f size)
{
	this->world_position = pos;
	this->geo_shape = sf::VertexArray(sf::Quads, 4);

	geo_shape[0].position = sf::Vector2f(pos.x - (size.x / 2.f), pos.y - (size.y / 2.f));
	geo_shape[1].position = sf::Vector2f(pos.x + (size.x / 2.f), pos.y - (size.y / 2.f));
	geo_shape[2].position = sf::Vector2f(pos.x + (size.x / 2.f), pos.y + (size.y / 2.f));
	geo_shape[3].position = sf::Vector2f(pos.x - (size.x / 2.f), pos.y + (size.y / 2.f));
	
	
	geo_shape[0].color = sf::Color::Red;
	geo_shape[1].color = sf::Color::Blue;
	geo_shape[2].color = sf::Color::Green;
	geo_shape[3].color = sf::Color::Yellow;

	std::cout << "Object created" << std::endl;
}

Object::~Object()
{
	std::cout << "Object deleted" << std::endl;
}

sf::Vector2f Object::getWorldPosition()
{
	return this->world_position;
}

sf::Vector2f Object::getVelocity()
{
	return this->velocity;
}

sf::VertexArray Object::getGeoShape()
{
	return this->geo_shape;
}

int Object::getRotation()
{
	return this->rotation;
}

void Object::setWorldPosition(sf::Vector2f pos)
{

}

void Object::setVelocity()
{

}

void Object::setGeoShape(sf::VertexArray shape)
{

}

void Object::setRotation(int rot)
{

}