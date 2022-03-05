#pragma once
#include "PhysicsAttribute.h"
#include "Object.h"
#include "HitboxNode.h"
#include <math.h>

/* HitboxData */
typedef struct _HBData
{
    HitboxNode* m_this = nullptr;
    HitboxNode* m_other = nullptr;
    sf::Vector2f m_this_resolve;
    INTERSECTED_SIDE m_this_side = INTERSECTED_SIDE::ODATA_NONE;
} HBData;

class CollisionHandler :
    public PhysicsAttribute
{
public: 
    CollisionHandler();
    ~CollisionHandler();

    //Getters

    //Setters

    //Others
    // Main function for separation. Calls the object seperation functions
    // below depending on if only one or both objects are moving.
    static std::vector<HBData> separateHitboxes(HitboxNode* i, HitboxNode* j);
    // Resolves collisions with only one moving object.
    static HBData singleSeparation(HitboxNode* moving, HitboxNode* still);
    // Resolves collisions where both objects are moving.
    static std::vector<HBData> dualSeparation(HitboxNode* i, HitboxNode* j);
    // Resolved collisions between a rectangle and non-rectangle SubBox.
    static std::vector<HBData> nonRectangleSeparation(HitboxNode* i, HitboxNode* j);
    // "Last resort" function that unstucks (separates) the CollisionHandleres of
    // two objects (one moving, one still). Moves the moving.CollisionHandler
    // the shortest distance (along x or y) that resolves the collision.
    static std::vector<sf::Vector2f> unstuck(sf::FloatRect moving, sf::FloatRect other, sf::Vector2f moving_vel);
    static sf::Vector2f unstuck(sf::FloatRect moving, sf::FloatRect other);
    // NOTE: DESCRIPTION HERE.
    static std::vector<sf::Vector2f> recalibrate(const HBData& odata, const sf::Vector2f& i_pos, const INTERSECTED_SIDE& adj_intersect, const sf::Vector2f& j_pos);

    // Checks for bounding box intersection between two HitboxNodes.
    static bool bb_intersects(sf::FloatRect i, sf::FloatRect j) { return (i.intersects(j) || j.intersects(i)); };
    static bool bb_intersects(HitboxNode* i, HitboxNode* j) { return bb_intersects(i->getBB(), j->getBB()); };
private:
    // Returns how much the two objects are overlapping each other,
    // along both the x- and the y-axis. (Used in dualObjectSeparation).
    static sf::Vector2f getOverlaps(sf::FloatRect i_rect, sf::FloatRect j_rect);
    // Simple checks to see if the two objects are travelling in parallell.
    static bool sameXDirection(Object* i, Object* j);
    static bool sameYDirection(Object* i, Object* j);
    // Calculates and returns the factors t_x and t_y that the object
    // x- and y-velocities must be multiplied by to undo the x- and y-
    // overlaps. (Used in dualObjectSeparation).
    static sf::Vector2f getT(HitboxNode* i, HitboxNode* j, sf::Vector2f overlaps);
};


//Temp revamp.
class CollHandler : public PhysicsAttribute
{
public:
    CollHandler();
    ~CollHandler();
    static std::vector<HBData> calculateResolves(HitboxNode* i, HitboxNode* j);
    static bool bb_intersects(const sf::FloatRect& i, const sf::FloatRect& j) { return (i.intersects(j) || j.intersects(i)); }
    static bool bb_intersects(HitboxNode* i, HitboxNode* j) { return bb_intersects(i->getBB(), j->getBB()); };
    static std::vector<sf::Vector2f> recalibrateResolves();
protected:
    static bool isDual(HitboxNode* i, HitboxNode* j);
    // Different resolving algorithms depending on the properties of the involved hitboxes.
    static std::vector<HBData> resolveAA(HitboxNode* i, HitboxNode* j);         // Axis-aligned.
    static std::vector<HBData> resolveSAT(HitboxNode* i, HitboxNode* j);        // Separating axes.
    static std::vector<HBData> resolveSpline(HitboxNode* i, HitboxNode* j);     // Vs (STATIC) spline hitbox.
    // Overlap calculations for different combinations of shapes. NOTE: The overlaps are calculated
    // in the transformed coordinate systems of i and j and then translated into the world coordinate
    // system.
    static sf::Vector2f calculateOverlaps(SubBox2* i, SubBox2* j);
    static sf::Vector2f ol_CircleCircle(CircleBox* i, CircleBox* j);
    static sf::Vector2f ol_RectRect(RectBox* i, RectBox* j);
    static sf::Vector2f ol_RectCircle(RectBox* i, CircleBox* j);
    static sf::Vector2f ol_RectConv(RectBox* i, ConvexBox* j);
    static sf::Vector2f ol_ConvConv(ConvexBox* i, ConvexBox* j);
    static sf::Vector2f ol_CircleConv(CircleBox* i, ConvexBox* j);
    static sf::Vector2f ol_RectSpline(RectBox* i, SplineBox* j);
    static sf::Vector2f ol_ConvSpline(ConvexBox* i, SplineBox* j);
    static sf::Vector2f ol_CircleSpline(CircleBox* i, SplineBox* j);
};
