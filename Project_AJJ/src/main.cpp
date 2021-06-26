#include <stdlib.h>
#include <iostream>
#include <cmath>
#include "Action.h"
#include "Object.h"
#include "Controller.h"
#include "ExtendedRenderWindow.h"
#include "Scene.h"
#include "Camera.h"
#include "CollisionDetection.h"
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
	groundTiles[2]->setVelocity(sf::Vector2f(0.01, 0.01));
	return testScene;
}

int main()
{
	sf::Event event;

	//Test environment
	ExtendedRenderWindow window(sf::Vector2u(800, 600), "Project AJJ");
	Scene* test_scene = drawTestScene();
	Camera scene_camera;
	CollisionDetection* col_det = new CollisionDetection(test_scene->getSceneObjects());
	test_scene->setCollisionDetection(col_det);
	test_scene->setCamera(&scene_camera);
	window.setActiveScene(test_scene);
	scene_camera.lockOnObject(test_scene->getSceneObjects()[2]);
	sf::Clock clock;
	sf::Time time;

	CollisionDetection colDec = CollisionDetection(test_scene->getSceneObjects());

	Controller contr;
	contr.setObject(test_scene->getSceneObjects()[2]);
	contr.bindActionToKey(contr.getObject()->getActions()[0], sf::Keyboard::Key::W);
	contr.bindActionToKey(contr.getObject()->getActions()[1], sf::Keyboard::Key::S);
	contr.bindActionToKey(contr.getObject()->getActions()[2], sf::Keyboard::Key::A);
	contr.bindActionToKey(contr.getObject()->getActions()[3], sf::Keyboard::Key::D);

	Controller contr2;
	contr2.setObject(test_scene->getSceneObjects()[3]);
	contr2.bindActionToKey(contr2.getObject()->getActions()[0], sf::Keyboard::Key::Up);
	contr2.bindActionToKey(contr2.getObject()->getActions()[1], sf::Keyboard::Key::Down);
	contr2.bindActionToKey(contr2.getObject()->getActions()[2], sf::Keyboard::Key::Left);
	contr2.bindActionToKey(contr2.getObject()->getActions()[3], sf::Keyboard::Key::Right);

	contr2.bindActionToKey(contr2.getObject()->getActions()[4], { sf::Keyboard::Key::Up,  sf::Keyboard::Key::Z, sf::Keyboard::Key::X, sf::Keyboard::Key::C });
	contr2.bindActionToKey(contr2.getObject()->getActions()[5], { sf::Keyboard::Key::Down, sf::Keyboard::Key::Z });
	contr2.bindActionToKey(contr2.getObject()->getActions()[6], { sf::Keyboard::Key::Left, sf::Keyboard::Key::Z });
	contr2.bindActionToKey(contr2.getObject()->getActions()[7], { sf::Keyboard::Key::Right, sf::Keyboard::Key::Z} );
	/////////////////////////////////////
	int i = 1;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) 
				{
					scene_camera.lockOnObject(test_scene->getSceneObjects()[(2 + (i % 2))]);
					i++;
				}
			}
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		contr.getUserInput();
		contr2.getUserInput();

		// clear the window with black color
		window.clear(sf::Color::Black);
		// draw everything here...
		time = clock.getElapsedTime();
		test_scene->getSceneObjects()[8]->setWorldPosition(sf::Vector2f(300.0f + 50*std::cos(time.asSeconds()), 300.0f + 50*std::sin(time.asSeconds())));
		//test_scene->getSceneObjects()[2]->onActionRequest("Left");
		//test_scene->getSceneObjects()[2]->onActionRequest("Up");
		window.drawActiveScene();

		// end the current frame
		window.display();
	}
	return 0;
}