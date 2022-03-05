#pragma once
#include "PropertyNode.h"

enum class HBOX { STATIC, DYNAMIC, HOLLOW, SEMI_STATIC, SEMI_DYNAMIC, SEMI_HOLLOW};
enum class HBSPLINE { LINEAR, CUBIC, NONE };
enum class RES_DIR { UP, RIGHT, DOWN, LEFT, ANY, HORI, VERT};

typedef struct _SubBox
{
    std::vector<sf::Vector2f> b_pts;               // Box points (in order of connection).
    HBSPLINE interpolation_type = HBSPLINE::NONE;  // Defines the curve inbetween points.
    bool endpoints_connected = false;              // Determines whether the first and the last points of b_pts should connect.
    RES_DIR push = RES_DIR::ANY;                   // Determines in which direction the colliding hitbox should be resolved
} SubBox;

class SubBox2;
class RectBox;
class CircleBox;
class ConvexBox;
class SplineBox;

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
    std::vector<sf::Vector2f> getSB_pts();
    // Returns a SubBox with the b_pts translated to world coordinates.
    SubBox getSB();
    SubBox2* getSB2();

    // Setters.
    void setBehavior(HBOX b) { this->box_behavior = b; };
    void setHitboxTotal(sf::Vector2f hb_center, sf::Vector2f hb_size);
    void setHitboxTotal(sf::FloatRect hb);
    void setHitboxRatio(sf::Vector2f hb_center, sf::Vector2f hb_size);
    void setHitboxRatio(sf::FloatRect hb_ratios);

    // Others.
    void updateObjectPos(sf::Vector2f hb_wp);
    bool createSubBox(std::vector<sf::Vector2f> pts);
    //bool connectTo(Object* parent) override;
    //bool disconnectFrom(Object* parent) override;

    void addSubBox(SubBox2* sb);

    // DEBUGGING.
    sf::VertexArray getDrawable();
    // END DEBUGGING.
private:
    sf::Vector2f bb_pos{ 0.f, 0.f };                      // Ratios.
    sf::Vector2f bb_size{ 0.f, 0.f };                     // Ratios.
    sf::FloatRect bounding_box{ 0.f, 0.f, 0.f, 0.f };     // The bounding rectangle that contains the Hitbox.
    std::vector<SubBox> sub_boxes;                        // Hitboxes within the bounding_box. If size = 0, bounding_box is treated as the hitbox.
    SubBox2* sb = nullptr;                                 //
    HBOX box_behavior{ HBOX::STATIC };                    // How a hitbox is expected to behave upon collision.
    // Data related to transform.
    float angle{ 0.0f };
};

/* Interface 
*/
class SubBox2
{
public:
    virtual ~SubBox2() {};
    // Getters.
    virtual sf::String getType() = 0;
    virtual sf::VertexArray getDrawable() = 0;
    virtual std::vector<sf::Vector2f> getVertices() = 0;
    // Setters.
    virtual void setLocalCenter(sf::Vector2f c) = 0;
    // Others.
    virtual void rescale() = 0;
protected:
    std::vector<sf::Vector2f> vertices;
    float angle{0.f};
};

/* 
*/
class RectBox : public SubBox2
{
public:
    RectBox();
    RectBox(sf::FloatRect rb);
    RectBox(sf::Vector2f center, sf::Vector2f size);
    ~RectBox();
    // Getters.
    sf::String getType() override { return sf::String("RectBox"); };
    sf::VertexArray getDrawable() override;
    std::vector<sf::Vector2f> getVertices() override;
    sf::FloatRect getRect();
    // Setters.
    void setLocalCenter(sf::Vector2f c) override;
    void setSize();
    void setRect();
    // Others.
    void rescale() override;
protected:
    sf::FloatRect sb_rect{ 0.f, 0.f, 1.f, 1.f };
};

class CircleBox : public SubBox2
{
public:
    CircleBox();
    CircleBox(sf::Vector2f c, float r);
    ~CircleBox();
    // Getters.
    sf::VertexArray getDrawable() override;
    std::vector<sf::Vector2f> getVertices() override;
    float getRadius();
    // Setters.
    void setLocalCenter(sf::Vector2f c) override;
    // Others.
    void rescale() override;
protected:
    sf::Vector2f center{0.5f, 0.5f};
    float radius{ 1.f };
};

class ConvexBox : public SubBox2
{
public:
    ConvexBox();
    ConvexBox(std::vector<sf::Vector2f> vertices);
    ~ConvexBox();
    // Getters.
    sf::VertexArray getDrawable() override;
    std::vector<sf::Vector2f> getVertices() override;
    // Setters.
    void setLocalCenter(sf::Vector2f) override;
    // Others.
    void connectEnds(bool b);
    void rescale() override;
protected:
    bool connected_ends{true};
};

class StaticSplineBox : public SubBox2
{
public:
    StaticSplineBox();
    ~StaticSplineBox();
    // Getters.
    
    // Setters.
    
    // Others.

protected:
    HBSPLINE spline_type{ HBSPLINE::NONE };
    RES_DIR resolve_dir{ RES_DIR::ANY };
};