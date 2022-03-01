#include "../includes/TestObject.h"

TestObject::TestObject(sf::Vector2f pos, sf::Vector2f size)
	: Object(pos, size)
{
    //------------For testing purposes---------------

    this->max_velocity = { 20.f,20.f };

    Action* idle = new Action;
    Action* actionUp = new Action;
    Action* actionDown = new Action;
    Action* actionLeft = new Action;
    Action* actionRight = new Action;
    Action* attack = new Action;

    //increase speed by:
    this->velocity_inc = 2.0f;

    idle->setActionName("Idle");
    idle->setParentObject(this);
    idle->setActionType(ACTIONTYPE::AT_IDLE);

    actionUp->setActionName("Up");
    actionUp->setParentObject(this);
    actionUp->setActionType(ACTIONTYPE::AT_MOVE_UP);
    actionUp->setParameterManipulation(-velocity_inc);

    actionDown->setActionName("Down");
    actionDown->setActionType(ACTIONTYPE::AT_MOVE_DOWN);
    actionDown->setParameterManipulation(velocity_inc);
    actionDown->setParentObject(this);

    actionLeft->setActionName("Left");
    actionLeft->setActionType(ACTIONTYPE::AT_MOVE_LEFT);
    actionLeft->setParameterManipulation(-velocity_inc);
    actionLeft->setParentObject(this);

    actionRight->setActionName("Right");
    actionRight->setActionType(ACTIONTYPE::AT_MOVE_RIGHT);
    actionRight->setParameterManipulation(velocity_inc);
    actionRight->setParentObject(this);

    attack->setActionName("Attack");
    attack->setActionType(ACTIONTYPE::AT_ATTACK);
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

    this->object_animations.insert(this->object_animations.begin(), {idle_anim, run_anim, atk_anim});

    //--------------------------------------------

    object_actions.push_back(idle);
    object_actions.push_back(actionUp);
    object_actions.push_back(actionDown);
    object_actions.push_back(actionLeft);
    object_actions.push_back(actionRight);
    object_actions.push_back(attack);

    // ---------------PhysicsAttribute test---------------

    this->behavior_type = OBJECT_BEHAVIOR::DYNAMIC;
    this->attributes.push_back("Gravity");
    this->attributes.push_back("Air Friction");

    HitboxNode* hb = new HitboxNode();
    hb->connectTo(this);
    hb->setBehavior(HBOX::DYNAMIC);
    hb->setHitboxRatio(sf::Vector2f(0.5, 0.6), sf::Vector2f(0.4, 0.8f));

    //--------------------------------------------
}

TestObject::TestObject(sf::Vector2f pos, sf::Vector2f size, bool npc)
    : Object(pos, size)
{
    this->max_velocity = { 5.f,5.f };

    //increase speed by:
    this->velocity_inc = 2.0f;

    this->behavior_type = OBJECT_BEHAVIOR::DYNAMIC;
    this->attributes.push_back("Gravity");
    this->attributes.push_back("Air Friction");
    
    this->setTextureName(sf::String("Boss"));
    Frame npc_frame;
    npc_frame.duration = 200;
    npc_frame.frame_index = 0;
    npc_frame.texture_id = TEXTURE_ID::NONE;
    npc_frame.region_name = sf::String("Boss");
    this->setCurrentFrame(npc_frame);


    HitboxNode* hb = new HitboxNode();
    hb->connectTo(this);
    hb->setBehavior(HBOX::DYNAMIC);
    hb->setHitboxRatio(sf::Vector2f(0.5, 0.5), sf::Vector2f(0.6, 0.8f));
}

TestObject::~TestObject()
{
}