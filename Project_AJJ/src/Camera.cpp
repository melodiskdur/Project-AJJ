#include "Camera.h"

Camera::Camera()
{
	camera_view = new sf::View(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
	camera_position = camera_view->getCenter();
	setCameraViewRect();
}

Camera::~Camera()
{
	delete camera_view;
	std::cout << "Camera deleted" << std::endl;
}

//Getters
sf::View* Camera::getCameraView()
{
	sf::Vector2f pos;
	if (target_object != nullptr)
		pos = target_object->getWorldPosition();
	else
		pos = this->getCameraPosition();
	setCameraPosition(pos);
	setCameraViewRect();
	return camera_view;
}

sf::Vector2f Camera::getCameraPosition()
{
	return camera_position;
}

sf::FloatRect Camera::getCameraViewRect()
{
	return camera_view_rect;
}

//Setters
void Camera::setCameraView(sf::View* view)
{
	camera_view = view;
	this->setCameraViewRect();
}

void Camera::setCameraPosition(sf::Vector2f pos)
{
	camera_position = pos;
	this->camera_view->setCenter(pos);
	this->setCameraViewRect();
}

void Camera::setCameraViewRect()
{
	float half_width = camera_view->getSize().x / 2;
	float half_height = camera_view->getSize().y / 2;
	sf::Vector2f upper_left = sf::Vector2f(camera_position.x - half_width, camera_position.y - half_height);
	camera_view_rect = sf::FloatRect(upper_left, camera_view->getSize());
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