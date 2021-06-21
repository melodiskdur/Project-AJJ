#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

/* Object 
Base class for all entities that exist within a scene. 
*/
class Object
{
public:
	Object(sf::Vector2f pos, sf::Vector2f size);
	virtual ~Object();

	//Getters
	sf::Vector2f getWorldPosition();
	sf::Vector2f getVelocity();
	sf::VertexArray getGeoShape();
	int getRotation();

	//Setters
	void setWorldPosition(sf::Vector2f pos);
	void setVelocity();
	void setGeoShape(sf::VertexArray shape);
	void setRotation(int rot);
	//
protected:
	sf::Vector2f world_position = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);
	sf::VertexArray geo_shape = sf::VertexArray(sf::Quads,4);	//Geometric boundary of the object
	sf::Vector2f size = sf::Vector2f(0.0f, 0.0f);				//Geo_shape size
	int rotation = 0;											//Rotation value of the object
	//Textures
	//Actions
	//PhysicsAttributes
private:
};