#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "Object.h"

#define MIN_LAYER_DEPTH 0.01f
#define MAX_LAYER_DEPTH 100.f
#define MIN_LAYER_ANGLE 0.f
#define MAX_LAYER_ANGLE 359.9f

/* SceneLayer (Abstract base class.)
   The SceneLayer is used to separate Objects and
*  other drawable entities into different sections based on when and
*  how we want to draw/render them. The class also allows for adding
*  multiple traits to a certain layer such as depth, scale, rotation. 
*  Traits that can then be used by the Scene and its Camera to manipulate
*  how the layer is shown in the Scene.
* 
*  Below are some sub-classes that allow for more flexibility depending
*  on what type of layer we need. All the functionality is handled internally
*  in the sense that the Scene only sees SceneLayer* instances. It doesn't
*  differentiate between the different subclasses.
* 
*  Note that layer_num = 0 is reserved for the main layer of the Scene. 
*/
class SceneLayer
{
public:
	SceneLayer();
	SceneLayer(int layer_num);
	SceneLayer(int layer_num, float depth, float scale);
	virtual ~SceneLayer();
	// Getters.
	float getDepth() { return this->depth; };
	float getScale() { return this->scale; };
	float getAngle() { return this->angle; };
	signed int getLayerNum() { return this->layer_num; };
	std::vector<Object*> getLayerObjects() { return this->layer_objects; };
	virtual std::vector<Object*> getLayerObjectsWithinView(sf::FloatRect view_rect);
	// Setters.
	void setDepth(float depth) { if (depth > MIN_LAYER_DEPTH && depth < MAX_LAYER_DEPTH) this->depth = depth; };
	void setScale(float scale) { if (scale > 0) this->scale = scale; };
	void setAngle(float angle) { if (angle > MIN_LAYER_ANGLE && angle < MAX_LAYER_ANGLE) this->angle = angle; };
	// Other functions.
	void toggleLayer(bool enabled) { this->enabled = enabled; };
	bool isEnabled() { return this->enabled; };
	virtual sf::View manipulateCameraView(const sf::View cam_v) { return cam_v; };
	virtual void addObject(Object* o) { this->layer_objects.push_back(o); };
	virtual void addObjects(std::vector<Object*> o_vec) { this->layer_objects.insert(this->layer_objects.end(), o_vec.begin(), o_vec.end()); };
	virtual void updateLayerObjects() {};

	static unsigned int instanceCount() { return instance_counter; };
protected:
	std::vector<Object*> layer_objects;	// The contents of the SceneLayer.		
	float depth = 1.f;					// How fast the layer should scroll in relation to the main layer. x > 1 slower. 0 < x < 1 faster. 
	float scale = 1.f;					// How the contents should be rescaled in relation to how they would appear in the main layer.
	float angle = 0.0f;					// The rotation of the layer in degrees.
	bool enabled = true;				// Decides whether the layer is enabled for drawing/rendering or not.
private:
	signed int layer_num = 1;			// Rendering order. x = 0: Main layer (reserved). x > 0 background, x < 0 : foreground.

	static unsigned int instance_counter;
};


/* StaticFixatedLayer
*  A Static and Fixated SceneLayer is the simplest SL-subclass. Think of it as providing a way to 
*  draw an image to a scene that's also locked to the viewport, like a skybox, a background color
*  or some static effect layer. Note that the objects added to a StaticFixatedLayer should be centered
*  around the coordinates (0,0). This is for ease of use, as the StaticFixatedLayer doesn't care about
*  there whereabouts of the Scene's Camera(s).
*  Note: Depth has no real purpose in a StaticFixatedLayer. However, you can still Scale things.
* */
class StaticFixatedLayer : public SceneLayer
{
public:
	StaticFixatedLayer(int layer_num);
	StaticFixatedLayer(int layer_num, float scale, sf::View cam_v);
	~StaticFixatedLayer() override;
	// Getters.
	std::vector<Object*> getLayerObjectsWithinView(sf::FloatRect view_rect) override { return this->obj_within_view; };
	// Setters.
	// Others.
	sf::View manipulateCameraView(const sf::View cam_v) override { return this->static_view; };
	void addObject(Object* o) override;
	void addObjects(std::vector<Object*> o_vec) override;
private:
	std::vector<Object*> obj_within_view;					// Since it's non-changing, we store away the viewable objects for quick call returns.
	sf::FloatRect static_rect = { {0.f, 0.f}, {0.f, 0.f} };	// Since the view area is also non-changing, store the view_rect.
	sf::View static_view;									// Same idea here.
};


/* StaticInteractiveLayer
*  A Static and Interactive SceneLayer as some perks that the StaticFixatedLayer does not have. It allows
*  the user to implement non-changing layers that still interact with the rest of the Scene in the sense
*  that its depth matters, aka scrolls to create parallax effects in the background/foreground of the main scene layer.
*  When the Camera moves, so does this SceneLayer. The Objects within do not change, however.
*/
class StaticInteractiveLayer : public SceneLayer
{
public:
	StaticInteractiveLayer(int layer_num);
	StaticInteractiveLayer(int layer_num, float scale, float depth);
	~StaticInteractiveLayer() override;
	// Getters.
	std::vector<Object*> getLayerObjectsWithinView(sf::FloatRect view_rect) override;
	// Setters.
	// Others.
	void addObject(Object* o) override;
	void addObjects(std::vector<Object*> o_vec) override;
	sf::View manipulateCameraView(const sf::View cam_v) override;
};


/* DynamicFixatedLayer 
*  Like Static Fixated SceneLayers, the DynamicFixated SceneLayer is also locked to the viewport.
*  Its Objects however, are not. This allows for creating active (yet not INTERactive layers), such as
*  animated scene effects, autoscrolling and repeating parallaxes etc. It being dynamic means that it can also be
*  affected by outside forces, e.g different triggers and events. 
*
*  Note: As with Static Fixated SceneLayers, the Objects within the DynamicFixated Layer are centered
*  around the coordinates (0,0). Depth does not matter here either.
*/
class DynamicFixatedLayer : public SceneLayer
{
public:
	DynamicFixatedLayer(int layer_num);
	DynamicFixatedLayer(int layer_num, float depth, float scale);
	~DynamicFixatedLayer() override;
	// Getters.
	std::vector<Object*> getLayerObjectsWithinView(sf::FloatRect view_rect) override;
	// Setters.
	void setAutoScrolling(sf::Vector2f a_s) { this->auto_scrolling = a_s; };
	// Others.
	sf::View manipulateCameraView(const sf::View cam_v) override;
	void addObject(Object* o) override;
	void addObjects(std::vector<Object*> o_vec) override;
	void updateLayerObjects() override;
private:
	bool repeat = true;
	bool fixated_x = true;
	bool fixated_y = false;
	sf::Vector2f auto_scrolling = { 0.0f, 0.0f };

	void checkForReset(Object* o, sf::FloatRect rect);
};


/* DynamicInteractiveLayer
*  The Dynamic Interactive SceneLayer is the most responsive. For all intents and purposes, the
*  Scene's main layer is a Dynamic Interactive Layer. It's basically all of the other sub-classes
*  wrapped into one. 
*/
class DynamicInteractiveLayer : public SceneLayer
{
public:
	DynamicInteractiveLayer(int layer_num);
	DynamicInteractiveLayer(int layer_num, float scale, float depth);
	~DynamicInteractiveLayer() override;
	// Getters.
	std::vector<Object*> getLayerObjectsWithinView(sf::FloatRect view_rect) override;
	// Setters
	void setScrollX(bool status) { this->scroll_x = status; };
	void setScrollY(bool status) { this->scroll_y = status; };
	// Others.
	void addObject(Object* o) override;
	void addObjects(std::vector<Object*> o_vec) override;
	void updateLayerObjects() override;
private:
	bool scroll_x = true;					// Layer scrolls along the x-axis when true.
	bool scroll_y = true;					// Layer scrolls along the y-axis when true.
};