#include <stdlib.h>
#include <iostream>
#include "Object.h"
#include "Controller.h"
#include "ExtendedRenderWindow.h"
#include "Scene.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>

int main()
{
	sf::Event event;

	//Test environment
	ExtendedRenderWindow window(sf::Vector2u(800, 600), "Project AJJ");
	Scene test_scene;
	Camera scene_camera;
	test_scene.setCamera(&scene_camera);
	Object obj(sf::Vector2f(100.f,100.f), sf::Vector2f(100.f, 100.f));
	test_scene.addSceneObject(&obj);
	window.setActiveScene(&test_scene);

	/////////////////////////////////////

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		window.drawActiveScene();

		// end the current frame
		window.display();
	}

	return 0;
}