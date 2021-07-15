#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include "Action.h"
#include "Animation.h"
#include "TextureIds.h"

class Action;

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
	sf::Vector2f getSize();
	sf::VertexArray getGeoShape();
	int getRotation();
	sf::String getTextureName();
	std::vector<Action*> getActions();
	TEXTURE_ID getTextureId() { return this->active_texture; };
	Frame getFrame() { return this->current_frame; };

	//Setters
	void setWorldPosition(sf::Vector2f pos);
	void setVelocity(sf::Vector2f vel);
	void setGeoShape(sf::VertexArray shape);
	void setRotation(int rot);
	void setTextureName(sf::String name);
	void setTextureId(TEXTURE_ID texture_id) { this->active_texture = texture_id; };
	void setFrame(Frame frame) { this->current_frame = frame; };
	void setColor(sf::Color color);
	//void setActions(std::vector<int> actions);

	//Others
	//Called when the object is queried for an action. Looks through its vector for the action 
	//with a name corresponding to std::string action_name.
	void onActionRequest(std::string action_name);

protected:

	//Parameters
	sf::Vector2f world_position = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f velocity = sf::Vector2f(1.f, 1.f);
	sf::VertexArray geo_shape = sf::VertexArray(sf::Quads,4);	//Geometric boundary of the object
	sf::Vector2f size = sf::Vector2f(0.0f, 0.0f);				//Geo_shape size
	int rotation = 0;											//Rotation value of the object

	//Actions
	std::vector<Action*> object_actions;

	//Textures
	sf::String object_texture_name = "";				//Name of the texture atlas that we want to draw frames of this object from.
	TEXTURE_ID active_texture = TEXTURE_ID::NONE;
	Frame current_frame;

	//PhysicsAttributes

private:
};