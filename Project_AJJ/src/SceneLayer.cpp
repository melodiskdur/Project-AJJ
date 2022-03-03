#include "SceneLayer.h"

unsigned int SceneLayer::instance_counter = 0;

/* * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                    SceneLayer                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
SceneLayer::SceneLayer() { SceneLayer::instance_counter++; }
SceneLayer::SceneLayer(int layer_num) : SceneLayer() { this->layer_num = layer_num; }

SceneLayer::SceneLayer(int layer_num, float depth, float scale) : SceneLayer()
{
    this->layer_num = layer_num;
    this->setDepth(depth);
    this->setScale(scale);
}

SceneLayer::~SceneLayer() { SceneLayer::instance_counter--; for (Object* o : this->layer_objects) delete o; }

std::vector<Object*> SceneLayer::getLayerObjectsWithinView(sf::FloatRect view_rect)
{
    std::vector<Object*> viewable_objects;
    // Scale view_rect in accordance to the SceneLayer depth and scale values.
    for (Object* o : this->layer_objects)
    {
        sf::FloatRect object_rect = { o->getWorldPosition(), o->getSize() };
        if (view_rect.intersects(object_rect)) viewable_objects.push_back(o);
    }
    return viewable_objects;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * * 
 *                              StaticFixatedLayer                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
StaticFixatedLayer::StaticFixatedLayer(int layer_num) : SceneLayer(layer_num) { }

StaticFixatedLayer::StaticFixatedLayer(int layer_num, float scale, sf::View cam_v) : SceneLayer(layer_num, 1.f, scale) 
{
    // Set up the static view. Point to (0,0).
    this->static_view = cam_v; 
    this->static_view.setCenter({ 0.f, 0.f });

    // Set up the view rect accordingly.
    this->static_rect = { this->static_view.getCenter() - 0.5f * this->static_view.getSize(), this->static_view.getSize() };
}

std::vector<Layout*> StaticFixatedLayer::getLayerLayoutsWithinView(sf::FloatRect view_rect)
{
    std::vector<Layout*> viewable_layouts;
    // Scale view_rect in accordance to the SceneLayer depth and scale values.
    for (Layout* l : this->layouts)
    {
        sf::FloatRect layout_rect = l->getRect();
        if (view_rect.intersects(layout_rect)) viewable_layouts.push_back(l);
    }
    return viewable_layouts;
}

StaticFixatedLayer::~StaticFixatedLayer() {}

void StaticFixatedLayer::addObject(Object* o)
{
    this->layer_objects.push_back(o);
    // Store in the viewable objects-vector if the layer's static view rect
    if (this->static_rect.intersects({ o->getWorldPosition(), o->getSize() })) this->obj_within_view.push_back(o);
}

void StaticFixatedLayer::addObjects(std::vector<Object*> o_vec)
{
    this->layer_objects.insert(this->layer_objects.end(), o_vec.begin(), o_vec.end());
    // Store the viewable objects in the viewable objects-vector.
    for (Object* o : o_vec)
        if (this->static_rect.intersects({ o->getWorldPosition(), o->getSize() }))
            this->obj_within_view.push_back(o);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/* * * * * * * * * * * * * * * * *  * * * * * * * * * * * * * * * * * * * * * * * * *
 *                             StaticInteractiveLayer                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **/
StaticInteractiveLayer::StaticInteractiveLayer(int layer_num) : SceneLayer(layer_num) { }

StaticInteractiveLayer::StaticInteractiveLayer(int layer_num, float scale, float depth) : SceneLayer(layer_num, depth, scale) { }

StaticInteractiveLayer::~StaticInteractiveLayer() {}

std::vector<Object*> StaticInteractiveLayer::getLayerObjectsWithinView(sf::FloatRect view_rect)
{
    std::vector<Object*> viewables;
    sf::View new_v(view_rect);
    new_v.zoom(this->depth * this->scale);
    view_rect = { new_v.getCenter() - 0.5f * new_v.getSize(), new_v.getSize() };
    for (Object* o : this->layer_objects)
    {
        if (view_rect.intersects({ o->getWorldPosition(), o->getSize() }))
            viewables.push_back(o);
    }
    return viewables;
}

void StaticInteractiveLayer::addObject(Object* o)
{
    // Rescale with regards to the SceneLayer's depth and scale values before adding.
    o->setWorldPosition(this->depth * this->scale * o->getWorldPosition());
    o->setSize(this->depth * this->scale * o->getSize());
    this->layer_objects.push_back(o);
}

void StaticInteractiveLayer::addObjects(std::vector<Object*> o_vec)
{
    for (Object* o : o_vec) this->addObject(o);
}

sf::View StaticInteractiveLayer::manipulateCameraView(const sf::View cam_v)
{
    sf::View manip_v = cam_v;
    manip_v.zoom(this->depth * this->scale);
    return manip_v;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                           DynamicFixatedLayer                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
DynamicFixatedLayer::DynamicFixatedLayer(int layer_num) : SceneLayer(layer_num) { }

DynamicFixatedLayer::DynamicFixatedLayer(int layer_num, float depth, float scale) : SceneLayer(layer_num, depth, scale) { }

DynamicFixatedLayer::~DynamicFixatedLayer() { }

std::vector<Object*> DynamicFixatedLayer::getLayerObjectsWithinView(sf::FloatRect view_rect)
{
    // Manipulate rectangle.
    sf::View v(view_rect);
    v.zoom(this->depth * this->scale);
    view_rect = { v.getCenter() - 0.5f * v.getSize(), v.getSize() };
    for (Object* o : this->layer_objects)
    {
        o->setWorldPosition(o->getWorldPosition() + this->auto_scrolling);
        if (this->repeat)
        {
            this->checkForReset(o, view_rect);
        }
    }
    return this->layer_objects;
}

sf::View DynamicFixatedLayer::manipulateCameraView(const sf::View cam_v)
{
    sf::View v = cam_v;
    v.zoom(this->depth * this->scale);
    return v;
}

void DynamicFixatedLayer::addObject(Object* o)
{
    // Rescale and store.
    o->setWorldPosition(this->depth * this->scale * o->getWorldPosition());
    o->setSize(this->depth * this->scale * o->getSize());
    this->layer_objects.push_back(o);
}

void DynamicFixatedLayer::addObjects(std::vector<Object*> o_vec) { for (Object* o : o_vec) this->addObject(o); }

void DynamicFixatedLayer::updateLayerObjects()
{
    /* * * * * * * * * * *
    *  INSERT STUFF HERE *
    * * * * * * * * * * **/
    // Autoscroll.
    
}

void DynamicFixatedLayer::checkForReset(Object* o, sf::FloatRect rect)
{
    if (!rect.intersects(sf::FloatRect(o->getWorldPosition(), o->getSize())))
    {
        sf::Vector2f o_wp = o->getWorldPosition();
        sf::Vector2f o_sz = o->getSize();
        // Check for line intersections.
        if (this->auto_scrolling.x > 0) o_wp.x = rect.left - o_sz.x + this->auto_scrolling.x;
        else if (this->auto_scrolling.x < 0) o_wp.x = rect.left + rect.width + this->auto_scrolling.x;
        else if (this->auto_scrolling.y > 0) o_wp.y = rect.top - o_sz.y + this->auto_scrolling.y;
        else if (this->auto_scrolling.y < 0) o_wp.y = rect.top + rect.height + this->auto_scrolling.y;
        o->setWorldPosition(o_wp);
    }
}



/* * * * * * * * * * * * *
* DynamicInteractiveLayer *
 * * * * * * * * * * * * */

