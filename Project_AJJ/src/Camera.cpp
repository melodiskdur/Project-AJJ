#include "Camera.h"

unsigned int Camera::instance_counter = 0;

Camera::Camera()
{
	Camera::instance_counter++;
	camera_view = new sf::View(sf::Vector2f(400, 300), sf::Vector2f(800, 600));
	camera_position = camera_view->getCenter();
	original_size = sf::Vector2f(800, 600);
	setCameraViewRect();
}

Camera::~Camera() 
{ 
	Camera::instance_counter--;
	if (camera_view != nullptr){ delete camera_view; camera_view = nullptr;}
}

//Getters
sf::View* Camera::getCameraView()
{
	sf::Vector2f pos;
	if (target_object != nullptr)
		pos = target_object->getWorldPosition();
	else
		pos = this->getCameraPosition();
	this->setCameraPosition(pos);
	this->setCameraViewRect();
	
	return camera_view;
}

sf::Vector2f Camera::getCameraPosition()
{
	return camera_position;
}

sf::FloatRect Camera::getCameraViewRect()
{
	return this->camera_view_rect;
}

float Camera::getCameraZoom()
{
	return this->current_zoom;
}

sf::View Camera::getManipulatedCameraView(float depth, float scale, float rotation)
{
	return this->getManipulatedCameraView(depth, scale, rotation, this->camera_view->getCenter());
}

sf::View Camera::getManipulatedCameraView(float depth, float scale, float rotation, sf::Vector2f position)
{
	if (this->camera_view == nullptr) return sf::View();
	sf::View manipulated_view = *(this->camera_view);
	// Resize the viewable area with regards to the depth.
	manipulated_view.zoom(depth);
	// Rotate the view with regards to the rotation.
	manipulated_view.rotate(rotation);
	// Reposition the view.
	manipulated_view.setCenter(position);
	return manipulated_view;
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
	this->camera_view_rect = sf::FloatRect(upper_left, this->camera_view->getSize());
}

void Camera::setCameraZoom(float zoom_factor)
{
	//NOTE:
	//Limits for the zoom are [0,2]
	//This is hardcoded for now. 

	//if the zoom_factor is the same as the current one, we dont bother updating
	if (zoom_factor == this->current_zoom) { return; }

	//make sure that the zoom_factor is within the limits
	if (zoom_factor > this->max_zoom)
	{
		zoom_factor = this->max_zoom;
	}
	else if (zoom_factor < 0)
	{
		zoom_factor = 0;
	}
	//zoom the camera_view
	this->camera_view->zoom(zoom_factor);
	//and update the current_zoom variable
	this->current_zoom = zoom_factor;
}

//Etc
void Camera::lockOnObject(Object* object)
{
	this->target_object = object;
}

void Camera::unlockFromObject()
{
	this->target_object = nullptr;
}

void Camera::addOrSubCameraZoom(float zoom_by_value) 
{
	setCameraZoom(this->current_zoom + zoom_by_value);
}

void Camera::resizeToViewPort(sf::Vector2f vp_sz)
{
	this->viewport_size = vp_sz;
	this->camera_view->setSize({ original_size.x * vp_sz.x, original_size.y * vp_sz.y });
	this->camera_view_rect.width = original_size.x * vp_sz.x;
	this->camera_view_rect.height = original_size.y * vp_sz.y;
}

