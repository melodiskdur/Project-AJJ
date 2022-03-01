#pragma once
#include "PropertyNode.h"

enum class HBOX { STATIC, DYNAMIC, HOLLOW, SEMI_STATIC, SEMI_DYNAMIC, SEMI_HOLLOW};

/* HitboxNode
*  Sub class to PropertyNode. Gives an easy way to attach a Hitbox to an Object, which is 
*  done simply by calling the connectTo()-method. The HitboxNode data is then used in the three
*  collision related classes (CollisionDetection/-Handler/-Graph) to check and resolve
*  collisions within a Scene.
*   
*  Internally the hitbox is determined as ratios starting from the upper left corner of the parent Object's geo_shape. 
*  However, for ease of use, the user can set the FloatRect(s) to be created from an 
*  arbitrary center point of the geo_shape with real values. There's also the option to define the FloatRect as a ratio of 
*  the parent Object's geo_shape.
* 
*  For STATIC objects, more advanced hitboxes can be created (defined as linear or other type of functions).
*  Interpolation can be used to connect parts/splines of an Object. This allows for e.g more interesting ground.
* 
*  Note that the stored coordinates are always local in relation to the parent Object's geo_shape. This
*  means that e.g (0, 0) corresponds to (left, top) of the parent.
*/
class HitboxNode :
    public PropertyNode
{
public:
    HitboxNode();
    ~HitboxNode();
    // Getters.
    HBOX getBehavior() { return this->box_behavior; };
    sf::Vector2f getLocalPos();
    sf::Vector2f getGlobalPos();
    sf::FloatRect getBB();

    // Setters.
    void setBehavior(HBOX b) { this->box_behavior = b; };
    void setHitboxTotal(sf::Vector2f hb_center, sf::Vector2f hb_size);
    void setHitboxTotal(sf::FloatRect hb);
    void setHitboxRatio(sf::Vector2f hb_center, sf::Vector2f hb_size);
    void setHitboxRatio(sf::FloatRect hb_ratios);

    // Others.
    void updateObjectPos(sf::Vector2f hb_wp);
    //bool connectTo(Object* parent) override;
    //bool disconnectFrom(Object* parent) override;

    sf::VertexArray getDrawable();
private:
    sf::Vector2f bb_pos{ 0.f, 0.f };                      // Ratios.
    sf::Vector2f bb_size{ 0.f, 0.f };                     // Ratios.
    sf::FloatRect bounding_box{ 0.f, 0.f, 0.f, 0.f };     // The hitbox which contains all other hitboxes.
    std::vector<sf::FloatRect> sub_boxes;                 // Hitboxes within the bounding_box. If size = 0, bounding_box is treated as a hitbox.
    HBOX box_behavior{ HBOX::STATIC };                    // How a hitbox is expected to behave upon collision.
    // Data related to transform.
    float angle{ 0.0f };
    sf::Vector2f rot_axis{ 0.f, 0.f };
    sf::Vector2f local_basis[2]{ {1.f, 0.f}, {0.f, 1.f} };
};