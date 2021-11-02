#include "Object.h"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <iostream>

Object::Object(sf::Vector2f pos, sf::Vector2f size)
{
    this->world_position = pos;
    this->geo_shape = sf::VertexArray(sf::Quads, 4);
    this->size = size;

    //add vertices to the quad shape
    geo_shape[0].position = sf::Vector2f(pos.x, pos.y);
    geo_shape[1].position = sf::Vector2f(pos.x + size.x, pos.y);
    geo_shape[2].position = sf::Vector2f(pos.x + size.x, pos.y + size.y);
    geo_shape[3].position = sf::Vector2f(pos.x, pos.y + size.y);

    //hardcoded example colors
    sf::Color gc = sf::Color::Black;
    geo_shape[0].color = gc;
    geo_shape[1].color = gc;
    geo_shape[2].color = gc;
    geo_shape[3].color = gc;

}

Object::Object()
{
    //empty constructor
}

Object::~Object()
{
    std::cout << "Object deleted" << std::endl;
}

int Object::getId()
{
    return this->obj_id;
}

sf::Vector2f Object::getWorldPosition()
{
    return this->world_position;
}

sf::Vector2f Object::getVelocity()
{
    return this->velocity;
}

sf::Vector2f Object::getSize()
{
    return this->size;
}

sf::VertexArray Object::getGeoShape()
{
    return this->geo_shape;
}

int Object::getRotation()
{
    return this->rotation;
}

sf::String Object::getTextureName()
{
    return object_texture_name;
}

std::vector<Action*> Object::getActions()
{
    return object_actions;
}

void Object::setId(int id)
{
    this->obj_id = id;
}

void Object::setWorldPosition(sf::Vector2f pos)
{
    //update object position and the quad shapes vertices
    this->world_position = pos;
    geo_shape[0].position = sf::Vector2f(pos.x, pos.y);
    geo_shape[1].position = sf::Vector2f(pos.x + size.x, pos.y);
    geo_shape[2].position = sf::Vector2f(pos.x + size.x, pos.y + size.y);
    geo_shape[3].position = sf::Vector2f(pos.x, pos.y + size.y);

    //Debugging
    sf::Vector2f vel = { this->velocity.x * 4, this->velocity.y * 4 };
    vel_vec[0][0].position = geo_shape[0].position;
    vel_vec[0][1].position = geo_shape[0].position + vel;
    vel_vec[1][0].position = geo_shape[1].position;
    vel_vec[1][1].position = geo_shape[1].position + vel;
    vel_vec[2][0].position = geo_shape[2].position;
    vel_vec[2][1].position = geo_shape[2].position + vel;
    vel_vec[3][0].position = geo_shape[3].position;
    vel_vec[3][1].position = geo_shape[3].position + vel;


}

void Object::setVelocity(sf::Vector2f vel)
{
    //set velocity
    this->velocity = vel;

    //check if the velocity is bigger than max
    //if so, lower it to the maximum velocity
    if (vel.x > this->max_velocity.x)
        this->velocity.x = this->max_velocity.x;
    else if (vel.x < -this->max_velocity.x)
        this->velocity.x = -this->max_velocity.x;

    if (vel.y > this->max_velocity.y)
        this->velocity.y = this->max_velocity.y;
    else if (vel.y < -this->max_velocity.y)
        this->velocity.y = -this->max_velocity.y;
    
    /*
    if ( sqrt(abs((vel.x * vel.x)) + abs((vel.y * vel.y))) > this->max_velocity.x )
    {
        float k = sqrt((this->max_velocity.x * this->max_velocity.x) / (abs((vel.x * vel.x)) + abs((vel.y * vel.y))) );
        this->velocity.x = round(this->velocity.x * k);
        this->velocity.y = round(this->velocity.y * k);
    }
    */
}

void Object::setGeoShape(sf::VertexArray shape)
{

}

void Object::setRotation(int rot)
{

}

void Object::setTextureName(sf::String name)
{
    object_texture_name = name;
}

void Object::setColor(sf::Color color)
{
    this->geo_shape[0].color = color;
    this->geo_shape[1].color = color;
    this->geo_shape[2].color = color;
    this->geo_shape[3].color = color;
}

//Others
void Object::onActionRequest(std::string action_name)
{
    //loop all actions assigned to this object
    for (int i = 0; i < object_actions.size(); i++)
    {
        //if the input action_name equals one action assigned to this object
        //then trigger that specific action and break the loop
        if (action_name.compare(object_actions[i]->getActionName()) == 0)
        {
            object_actions[i]->triggerAction();
            //this->setWorldPosition(world_position);
            break;
        }
    }
}