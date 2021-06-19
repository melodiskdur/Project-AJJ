#include "Camera.h"

Camera::Camera()
{
	camera_view = new sf::View(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
}

Camera::~Camera()
{
	delete camera_view;
	std::cout << "Camera deleted" << std::endl;
}

//Getters
sf::View* Camera::getCameraView()
{
	if (target_object != nullptr)
		camera_view->setCenter(target_object->getWorldPosition());
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
	this->camera_view->setCenter(pos);
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