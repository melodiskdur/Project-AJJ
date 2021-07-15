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
    geo_shape[0].color = sf::Color::Red;
    geo_shape[1].color = sf::Color::Blue;
    geo_shape[2].color = sf::Color::Green;
    geo_shape[3].color = sf::Color::Yellow;

    std::cout << "Object created" << std::endl;

    //For testing purposes
    Action * actionUp = new Action;
    Action* actionDown = new Action;
    Action* actionLeft = new Action;
    Action* actionRight = new Action;

    Action* speedUp = new Action;
    Action* speedDown = new Action;
    Action* speedLeft = new Action;
    Action* speedRight = new Action;

    actionUp->setActionName("Up");
    actionUp->setParentObject(this);
    actionUp->setActionParameter(&this->world_position.y);
    actionUp->setParameterManipulation(-this->velocity.y);

    actionDown->setActionName("Down");
    actionDown->setActionParameter(&this->world_position.y);
    actionDown->setParameterManipulation(this->velocity.y);
    actionDown->setParentObject(this);

    actionLeft->setActionName("Left");
    actionLeft->setActionParameter(&this->world_position.x);
    actionLeft->setParameterManipulation(-this->velocity.x);
    actionLeft->setParentObject(this);

    actionRight->setActionName("Right");
    actionRight->setActionParameter(&this->world_position.x);
    actionRight->setParameterManipulation(this->velocity.x);
    actionRight->setParentObject(this);

    //---------------Animation test---------------
    Animation* a_test = new Animation();
    a_test->addFrame(TEXTURE_ID::RUN, 0, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 1, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 2, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 3, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 4, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 5, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 6, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 7, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 8, 200.0f);
    a_test->addFrame(TEXTURE_ID::RUN, 9, 200.0f);
    actionRight->setAnimation(a_test);
    actionLeft->setAnimation(a_test);
    actionUp->setAnimation(a_test);
    actionDown->setAnimation(a_test);
    //--------------------------------------------

    speedUp->setActionName("SpeedUp");
    speedUp->setParentObject(this);
    speedUp->setActionParameter(&this->world_position.y);
    speedUp->setParameterManipulation(2*-this->velocity.y);

    speedDown->setActionName("SpeedDown");
    speedDown->setParentObject(this);
    speedDown->setActionParameter(&this->world_position.y);
    speedDown->setParameterManipulation(2*this->velocity.y);

    speedLeft->setActionName("SpeedLeft");
    speedLeft->setParentObject(this);
    speedLeft->setActionParameter(&this->world_position.x);
    speedLeft->setParameterManipulation(-2*this->velocity.x);

    speedRight->setActionName("SpeedRight");
    speedRight->setParentObject(this);
    speedRight->setActionParameter(&this->world_position.x);
    speedRight->setParameterManipulation(2*this->velocity.x);

    object_actions.push_back(actionUp);
    object_actions.push_back(actionDown);
    object_actions.push_back(actionLeft);
    object_actions.push_back(actionRight);
    object_actions.push_back(speedUp);
    object_actions.push_back(speedDown);
    object_actions.push_back(speedLeft);
    object_actions.push_back(speedRight);
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

void Object::setWorldPosition(sf::Vector2f pos)
{
	//update object position and the quad shapes vertices
	this->world_position = pos;
	geo_shape[0].position = sf::Vector2f(pos.x, pos.y);
	geo_shape[1].position = sf::Vector2f(pos.x + size.x, pos.y);
	geo_shape[2].position = sf::Vector2f(pos.x + size.x, pos.y + size.y);
	geo_shape[3].position = sf::Vector2f(pos.x, pos.y + size.y);
}

void Object::setVelocity(sf::Vector2f vel)
{
    this->velocity = vel;
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

    for (int i = 0; i < object_actions.size(); i++)
    {
        if (action_name.compare(object_actions[i]->getActionName()) == 0)
        {
            object_actions[i]->triggerAction();
            //this->setWorldPosition(world_position);
            break;
        }
    }
}