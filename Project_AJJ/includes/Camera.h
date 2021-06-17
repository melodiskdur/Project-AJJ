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

	//Setters
	void setCameraView(sf::View* view);
	void setCameraPosition(sf::Vector2f pos);

	//
	//Allows the camera to follow an object.
	void lockOnObject(Object* object);
	//Allows the camera to stop following an object.
	void unlockFromObject();
private:
	sf::View* camera_view;				// Display region.
	sf::Vector2f camera_position;		// Camera central point within a scene.
	Object* target_object = nullptr;	// Object to follow.
};

