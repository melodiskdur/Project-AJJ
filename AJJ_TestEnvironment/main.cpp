#include "Project_AJJ.h"
#include "includes/TestObject.h"

/*KOMMENTARER
* La till TextureIds som är en enumclass där varje namn motsvarar ett område i atlasen
* Se TextureAtlas.cpp för dessa områden. På detta sätt behöver vi endast länka textureId till
* Object då den inte behöver hela textureatlas.
* Den enda klasss som har TextureAtlas är ExtendedRenderWindow
* då det är den som ritar ut och därmed behöver hämta sprites.
* Så Object ha ett TextureId, Action en animation som updateras i triggerAction().
* Kanske något vi kan göra med actiontype sen också.
*
* I object har jag skapat en testanimation med några sprites och kopplat den till
* right,left,up,down. Du ser därför ingen sprite när den står stilla i början
* Får göra en till action som är när inga knappar trycks ned för att få en Idle/Standby läge
*
* Det som behöver implementeras är att duration för varje frame tas med
* Skrev mer om det i Animation::update(), kolla där.
*
* Tror det var allt...
* RADERA DETTA MEDDELANDE EFTER LÄSNING!!! VIKTIGT
*
*/

Scene* drawTestScene()
{
	Scene* testScene = new Scene();

	std::vector<Object*> groundTiles;
	for (int i = 0; i < 8; i++)
	{
		groundTiles.push_back(new TestObject(sf::Vector2f(i * 100.0f, 500.0f), sf::Vector2f(60.f, 100.f)));
	}
	testScene->addSceneObjects(groundTiles);
	TestObject* anotherObject = new TestObject(sf::Vector2f(300.0f, 300.0f), sf::Vector2f(100.f, 100.f));
	testScene->addSceneObject(anotherObject);
	groundTiles[2]->setVelocity(sf::Vector2f(0, 0));
	return testScene;
}

int main()
{
	sf::Event event;

	//Test environment
	ExtendedRenderWindow window(sf::Vector2u(800, 600), "Project AJJ");
	window.setFramerateLimit(140);

	//------------TextureAtlas test---------------
	TextureManager* tex_mag = new TextureManager();
	tex_mag->loadAtlas("Rogue", "../Project_AJJ/assets/rogue_atlas.png");
	TextureAtlas* robot = tex_mag->getAtlas("Rogue");
	robot->createRegionGrid(10, 10);
	robot->assignTextureId(TEXTURE_ID::RUN, sf::Vector2u(0, 2), sf::Vector2u(9, 2));
	robot->assignTextureId(TEXTURE_ID::IDLE, sf::Vector2u(0, 5), sf::Vector2u(9, 5));
	robot->assignTextureId(TEXTURE_ID::ATTACK, sf::Vector2u(0, 3), sf::Vector2u(9, 3));
	window.setTextureManager(tex_mag);
	//--------------------------------------------

	Scene* test_scene = drawTestScene();
	Camera scene_camera;
	CollisionDetection* col_det = new CollisionDetection(test_scene->getSceneObjects());
	test_scene->setCollisionDetection(col_det);
	test_scene->setCamera(&scene_camera);
	window.setActiveScene(test_scene);
	scene_camera.lockOnObject(test_scene->getSceneObjects()[2]);
	sf::Clock clock;
	sf::Time time;

	//-----------------------Add Atlas Name to Objects---------------------
	for (int i = 0; i < test_scene->getSceneObjects().size(); i++)
	{
		test_scene->getSceneObjects()[i]->setTextureName("Rogue");
	}

	Controller contr;
	contr.setObject(test_scene->getSceneObjects()[2]);
	contr.bindActionToKey(contr.getObject()->getActions()[0], sf::Keyboard::Key::Unknown); //IDLE
	contr.bindActionToKey(contr.getObject()->getActions()[1], sf::Keyboard::Key::W);
	contr.bindActionToKey(contr.getObject()->getActions()[2], sf::Keyboard::Key::S);
	contr.bindActionToKey(contr.getObject()->getActions()[3], sf::Keyboard::Key::A);
	contr.bindActionToKey(contr.getObject()->getActions()[4], sf::Keyboard::Key::D);

	contr.bindActionToKey(contr.getObject()->getActions()[5], sf::Keyboard::Key::LShift); //FUN-TEST ATTACK
	

	Controller contr2;
	contr2.setObject(test_scene->getSceneObjects()[3]);
	contr2.bindActionToKey(contr2.getObject()->getActions()[0], sf::Keyboard::Key::Unknown); //IDLE
	contr2.bindActionToKey(contr2.getObject()->getActions()[1], sf::Keyboard::Key::Up);
	contr2.bindActionToKey(contr2.getObject()->getActions()[2], sf::Keyboard::Key::Down);
	contr2.bindActionToKey(contr2.getObject()->getActions()[3], sf::Keyboard::Key::Left);
	contr2.bindActionToKey(contr2.getObject()->getActions()[4], sf::Keyboard::Key::Right);
	contr2.bindActionToKey(contr2.getObject()->getActions()[5], sf::Keyboard::Key::RShift); //FUN-TEST ATTACK

	contr2.bindActionToKey(contr2.getObject()->getActions()[6], { sf::Keyboard::Key::Up,  sf::Keyboard::Key::L });
	contr2.bindActionToKey(contr2.getObject()->getActions()[7], { sf::Keyboard::Key::Down, sf::Keyboard::Key::L });
	contr2.bindActionToKey(contr2.getObject()->getActions()[8], { sf::Keyboard::Key::Left, sf::Keyboard::Key::L });
	contr2.bindActionToKey(contr2.getObject()->getActions()[9], { sf::Keyboard::Key::Right, sf::Keyboard::Key::L });
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

		contr.processUserInput();
		contr2.processUserInput();

		// clear the window with black color
		window.clear(sf::Color::Black);
		// draw everything here...
		time = clock.getElapsedTime();
		test_scene->getSceneObjects()[8]->setWorldPosition(sf::Vector2f(300.0f + 50 * std::cos(time.asSeconds()), 300.0f + 50 * std::sin(time.asSeconds())));
		window.drawActiveScene();

		// end the current frame
		window.display();
	}
	delete col_det;
	delete tex_mag;
	return 0;
}