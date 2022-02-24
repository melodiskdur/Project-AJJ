#pragma once
#include "PropertyNode.h"

enum class HBOX { STATIC, DYNAMIC, HOLLOW, SEMI_STATIC, SEMI_DYNAMIC, SEMI_HOLLOW};

class HitboxNode :
    public PropertyNode
{
public:
    HitboxNode();
    ~HitboxNode();
    // Getters.
    HBOX getBehavior() { return this->box_behavior; };
    // Setters.
    void setBehavior(HBOX b) { this->box_behavior = b; };
    // Others.
    bool connectTo(Object* parent) override;
    bool disconnectFrom(Object* parent) override;
private:
    sf::FloatRect bounding_box = { 0.f, 0.f, 0.f, 0.f };
    std::vector<sf::FloatRect> sub_boxes;
    HBOX box_behavior = HBOX::STATIC;
    float angle = 0.0f;
    sf::Vector2f rot_axis = { 0.f, 0.f };
    sf::Vector2f local_basis[2] = { {1.f, 0.f}, {0.f, 1.f} };
};