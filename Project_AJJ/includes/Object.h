#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include "Action.h"
#include "Animation.h"
#include "TextureIds.h"
#include "PropertyNode.h"

enum class OBJECT_BEHAVIOR { STATIC, DYNAMIC, HOLLOW };

class Action;
class PropertyNode;

/* Object 
Base class for all entities that exist within a scene. 
*/
class Object
{
public:
	Object();
	Object(sf::Vector2f pos, sf::Vector2f size);
	virtual ~Object();

	//Getters
	int getId();
	sf::String getClassId() { return this->class_id; }
	sf::Vector2f getWorldPosition();
	sf::Vector2f getVelocity();
	sf::Vector2f getMaxVelocity() { return this->max_velocity; };
	sf::Vector2f getSize();
	sf::VertexArray getGeoShape();
	int getRotation();
	sf::String getTextureName();
	std::vector<Action*> getActions();
	TEXTURE_ID getTextureId() { return this->active_texture; };
	Frame getCurrentFrame() { return this->current_frame; };
	float getVelocityIncrement() { return this->velocity_inc; };
	std::vector<std::string> getPhysicsAttributes() { return this->attributes; };
	OBJECT_BEHAVIOR getBehaviorType() { return this->behavior_type; };
	sf::FloatRect getFloatRect();
	int getNumActions() { return this->num_actions; };
	std::vector<PropertyNode*> getPropertyNodes() { return this->property_nodes; };

	//Setters
	void setId(int id);
	void setClassId(sf::String new_class_id);
	void setWorldPosition(sf::Vector2f pos);
	void setVelocity(sf::Vector2f vel);
	void setMaxVelocity(sf::Vector2f max_vel) { this->max_velocity = max_vel; };
	void setSize(sf::Vector2f size);
	void setGeoShape(sf::VertexArray shape);
	void setRotation(int rot);
	void setTextureName(sf::String name);
	void setTextureId(TEXTURE_ID texture_id) { this->active_texture = texture_id; };
	void setCurrentFrame(Frame frame) { this->current_frame = frame; };
	void setColor(sf::Color color);
	void setVelocityIncrement(float vel_inc) { this->velocity_inc = vel_inc; };
	//void setActions(std::vector<int> actions);
	void setBehaviorType(OBJECT_BEHAVIOR behavior) { this->behavior_type = behavior;  };


	//Others
	//Called when the object is queried for an action. Looks through its vector for the action 
	//with a name corresponding to std::string action_name.
	void onActionRequest(std::string action_name);
	void addPropertyNode(PropertyNode* n) { this->property_nodes.push_back(n); };
	void addAction(Action* action) { this->object_actions.push_back(action); num_actions++;}

	//DEBUGGING
	sf::VertexArray vel_vec[4] = { sf::VertexArray(sf::Lines, 2), //UR
								 sf::VertexArray(sf::Lines, 2), //UL
								 sf::VertexArray(sf::Lines, 2), //LL
								 sf::VertexArray(sf::Lines, 2) }; //LR

	static unsigned int instanceCount() { return instance_counter; };
protected:
	//Parameters
	int obj_id = 0;												//object id
	sf::String class_id = "default";							//object class
	sf::Vector2f world_position = sf::Vector2f(0.0f, 0.0f);		//Position on-screen
	sf::Vector2f velocity = sf::Vector2f(0.0f, 0.0f);			//Current velocity
	float velocity_inc = 0.0f;									//Velocity increment
	sf::Vector2f max_velocity = sf::Vector2f(0.0f, 0.0f);		//Maximum velocity
	sf::VertexArray geo_shape = sf::VertexArray(sf::Quads, 4);	//Geometric boundary of the object
	sf::Vector2f size = sf::Vector2f(0.0f, 0.0f);				//Geo_shape size
	int rotation = 0;											//Rotation value of the object

	// Properties.
	std::vector<PropertyNode*> property_nodes;
	
	//Actions
	std::vector<Action*> object_actions;
	int num_actions = 0;

	//Animations
	std::vector<Animation*> object_animations;

	//Textures
	sf::String object_texture_name = "";				//Name of the texture atlas that we want to draw frames of this object from.
	TEXTURE_ID active_texture = TEXTURE_ID::NONE;
	Frame current_frame;

	//PhysicsAttributes
	OBJECT_BEHAVIOR behavior_type = OBJECT_BEHAVIOR::STATIC;  // Physical behavior of object. Default is static.
	std::vector<std::string> attributes;				      // The Physics Attribute types that the Object should be affected by.

private:
	static unsigned int instance_counter;
};