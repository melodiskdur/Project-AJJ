#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Object.h"

/* Camera
*	Contains camera properties. Can be placed in a Scene as pseudo object. 
*   It will then be able to transfer a certain region of the Scene (what we want to 
    see on screen) to the sf::View* camera_view member. By assigning the view to a 
    render target (ExtendedRenderWindow recommended as it can handle Scene instances 
	directly) the contents of the Scene can finally be drawn and displayed.
*/
class Camera
{
public:
	Camera();
	~Camera();

	//Getters
	sf::View* getCameraView();
	sf::Vector2f getCameraPosition();
	sf::FloatRect getCameraViewRect();
	float getCameraZoom();
	Object* getTargetObject() { return this->target_object; };
	// Copies the current camera view and manipulates it with respect to given
	// parameters.
	sf::View getManipulatedCameraView(float depth, float scale, float rotation);
	sf::View getManipulatedCameraView(float depth, float scale, float rotation, sf::Vector2f position);

	//Setters
	void setCameraView(sf::View* view);
	void setCameraPosition(sf::Vector2f pos);
	void setCameraViewRect();
	void setCameraZoom(float zoom_factor);

	//Allows the camera to follow an object.
	void lockOnObject(Object* object);
	//Allows the camera to stop following an object.
	void unlockFromObject();
	//Zoom out/in with the camera
	void addOrSubCameraZoom(float zoom_factor);
	

private:
	sf::View* camera_view;				// Display region.
	sf::Vector2f camera_position;		// Camera central point within a scene.
	sf::FloatRect camera_view_rect;		// The part of the scene that is seen from the camera POV.
	float current_zoom = 1;				// Default = 1. If z_f > 1, objects appear smaller.
	float max_zoom = 2;					// Maximum allowed zoom.
	Object* target_object = nullptr;	// Object to follow.
};

