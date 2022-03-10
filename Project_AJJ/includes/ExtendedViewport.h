#pragma once
#include "Camera.h"

class ExtendedViewport
{
public:
	ExtendedViewport() {ExtendedViewport::instance_counter++;};
	ExtendedViewport(Camera* cam, sf::FloatRect viewportRect);
	~ExtendedViewport();

	//getters
	Camera* getCamera();
	sf::FloatRect getFloatRect();


	void connect(Camera*);
	Camera* disconnect();

private:
	Camera* cam = nullptr;
	sf::FloatRect viewportRect{0.f, 0.f, 1.f, 1.f};






	static unsigned int instance_counter;

};

