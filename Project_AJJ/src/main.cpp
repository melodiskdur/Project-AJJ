#include <stdlib.h>
#include <iostream>
#include "Object.h"
#include "Controller.h"
#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Project AJJ");

	sf::Event event;

	Object obj(sf::Vector2f(100.f,100.f), sf::Vector2f(100.f, 100.f));

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
		// window.draw(...);
		
		window.draw(obj.getGeoShape());

		// end the current frame
		window.display();
	}

	return 0;
}