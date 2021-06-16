#include "Camera.h"

Camera::Camera()
{
	camera_view = new sf::View();
}

Camera::~Camera()
{
	delete camera_view;
}

//Getters
sf::View* Camera::getCameraView()
{
	return camera_view;
}

sf::Vector2f Camera::getCameraPosition()
{
	return camera_position;
}

//Setters
void Camera::setCameraView(sf::View* view)
{
	camera_view = view;
}

void Camera::setCameraPosition(sf::Vector2f pos)
{
	camera_position = pos;
}

//Etc
void Camera::lockOnObject(Object* object)
{
	target_object = object;
}

void Camera::unlockFromObject()
{
	target_object = nullptr;
}