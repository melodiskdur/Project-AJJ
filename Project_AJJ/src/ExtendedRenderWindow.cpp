#include "ExtendedRenderWindow.h"

ExtendedRenderWindow::ExtendedRenderWindow(sf::Vector2u resolution, std::string window_title)
	: sf::RenderWindow(sf::VideoMode(resolution.x, resolution.y), window_title)
{

}

ExtendedRenderWindow::~ExtendedRenderWindow()
{
	std::cout << "ExtendedRenderWindow deleted" << std::endl;
}

//Getters
Scene* ExtendedRenderWindow::getActiveScene()
{
	return active_scene;
}

//Setters
void ExtendedRenderWindow::setActiveScene(Scene* scene)
{
	active_scene = scene;
}

void ExtendedRenderWindow::drawActiveScene()
{
	if (active_scene != nullptr)
	{
		for (Object* o : active_scene->getSceneObjects())
		{
			this->draw(o->getGeoShape());
		}
	}
}