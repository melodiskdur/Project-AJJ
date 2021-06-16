#pragma once
#include <SFML/Graphics.hpp>
#include "Object.h"

/*
	Camera class
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
	//Etc
	void lockOnObject(Object* object);
	void unlockFromObject();
private:
	sf::View* camera_view;
	sf::Vector2f camera_position;
	Object* target_object = nullptr;
};

