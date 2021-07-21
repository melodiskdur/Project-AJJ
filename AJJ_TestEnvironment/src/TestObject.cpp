#include "../includes/TestObject.h"

TestObject::TestObject(sf::Vector2f pos, sf::Vector2f size)
	: Object(pos, size)
{
    //------------For testing purposes---------------

    this->max_velocity = { 3.f,3.f };

    Action* idle = new Action;
    Action* actionUp = new Action;
    Action* actionDown = new Action;
    Action* actionLeft = new Action;
    Action* actionRight = new Action;
    Action* attack = new Action;
    
    Action* speedUp = new Action;
    Action* speedDown = new Action;
    Action* speedLeft = new Action;
    Action* speedRight = new Action;

    //increase speed by:
    float vel_inc = 0.1f;

    idle->setActionName("Idle");
    idle->setParentObject(this);

    actionUp->setActionName("Up");
    actionUp->setParentObject(this);
    actionUp->setActionParameter(&this->world_position.y);
    actionUp->setParameterManipulation(-vel_inc);

    actionDown->setActionName("Down");
    actionDown->setActionParameter(&this->world_position.y);
    actionDown->setParameterManipulation(vel_inc);
    actionDown->setParentObject(this);

    actionLeft->setActionName("Left");
    actionLeft->setActionParameter(&this->world_position.x);
    actionLeft->setParameterManipulation(-vel_inc);
    actionLeft->setParentObject(this);

    actionRight->setActionName("Right");
    actionRight->setActionParameter(&this->world_position.x);
    actionRight->setParameterManipulation(vel_inc);
    actionRight->setParentObject(this);

    attack->setActionName("Attack");
    attack->setParentObject(this);
  
    //---------------Animation test---------------
    
    Animation* idle_anim = new Animation(TEXTURE_ID::IDLE, 0, 9, 200.0f);
    idle->setAnimation(idle_anim);
    
    Animation* run_anim = new Animation(TEXTURE_ID::RUN, 0, 9, 200.0f);
    actionRight->setAnimation(run_anim);
    actionLeft->setAnimation(run_anim);
    actionUp->setAnimation(run_anim);
    actionDown->setAnimation(run_anim);

    Animation* atk_anim = new Animation(TEXTURE_ID::ATTACK, 0, 9, 50.0f);
    attack->setAnimation(atk_anim);

    //--------------------------------------------

    speedUp->setActionName("SpeedUp");
    speedUp->setParentObject(this);
    speedUp->setActionParameter(&this->world_position.y);
    speedUp->setParameterManipulation(2 * -vel_inc);

    speedDown->setActionName("SpeedDown");
    speedDown->setParentObject(this);
    speedDown->setActionParameter(&this->world_position.y);
    speedDown->setParameterManipulation(2 * vel_inc);

    speedLeft->setActionName("SpeedLeft");
    speedLeft->setParentObject(this);
    speedLeft->setActionParameter(&this->world_position.x);
    speedLeft->setParameterManipulation(-2 * vel_inc);

    speedRight->setActionName("SpeedRight");
    speedRight->setParentObject(this);
    speedRight->setActionParameter(&this->world_position.x);
    speedRight->setParameterManipulation(2 * vel_inc);

    object_actions.push_back(idle);
    object_actions.push_back(actionUp);
    object_actions.push_back(actionDown);
    object_actions.push_back(actionLeft);
    object_actions.push_back(actionRight);
    object_actions.push_back(attack);
    
    object_actions.push_back(speedUp);
    object_actions.push_back(speedDown);
    object_actions.push_back(speedLeft);
    object_actions.push_back(speedRight);

	std::cout << "TestObject called\n";
}

TestObject::~TestObject()
{
}