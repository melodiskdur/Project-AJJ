#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "Object.h"
#include "Controller.h"
#include "ExtendedRenderWindow.h"
#include "Scene.h"
#include "Camera.h"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

Scene* drawTestScene()
{
	Scene* testScene = new Scene();
	std::vector<Object*> groundTiles;
	for (int i = 0; i < 8; i++)
	{
		groundTiles.push_back(new Object(sf::Vector2f(i*100.0f, 500.0f), sf::Vector2f(60.f, 100.f)));
	}
	testScene->addSceneObjects(groundTiles);
	Object* anotherObject = new Object(sf::Vector2f(300.0f, 300.0f), sf::Vector2f(100.f, 100.f));
	testScene->addSceneObject(anotherObject);
	return testScene;
}

int main()
{
	sf::Event event;

	//Test environment
	ExtendedRenderWindow window(sf::Vector2u(800, 600), "Project AJJ");
	Scene* test_scene = drawTestScene();
	Camera scene_camera;
	test_scene->setCamera(&scene_camera);
	window.setActiveScene(test_scene);
	scene_camera.lockOnObject(test_scene->getSceneObjects()[8]);
	sf::Clock clock;
	sf::Time time;

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
		time = clock.getElapsedTime();
		test_scene->getSceneObjects()[8]->setWorldPosition(sf::Vector2f(300.0f + 50*std::cos(time.asSeconds()), 300.0f + 50*std::sin(time.asSeconds())));
		window.drawActiveScene();

		// end the current frame
		window.display();
	}
	return 0;
}