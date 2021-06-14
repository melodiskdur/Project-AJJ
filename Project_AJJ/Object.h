#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>

/* Object 
Base class for all entities that exist within a scene. 
*/
class Object
{
public:
	Object();
	virtual ~Object();
	//Getters
	sf::Vector2f getWorldPosition();
	sf::VertexArray getGeoShape();
	int getRotation();
	//Setters
	void setWorldPosition(sf::Vector2f pos);
	void setGeoShape(sf::VertexArray shape);
	void setRotation(int rot);
	//
protected:
	sf::Vector2f world_position = sf::Vector2f(0.0f, 0.0f);
	sf::VertexArray geo_shape;									//Geometric boundary of the object
	int rotation = 0;											//Rotation value of the object
	//Textures
	//Actions
	//Animations
	//PhysicsAttributes
private:
};