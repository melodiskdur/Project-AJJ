#include "Project_AJJ.h"
#include "includes/TestObject.h"
#include "includes/FirstScene.h"
#include "includes/ResourceLoader.h"
#include "includes/TestMainControllerObject.h"
#include "includes//MainController.h"

int main()
{
	sf::Event event;

	//Test environment
	float sc_f = 1; //scale_factor for window width/height
	float window_width = 800 * sc_f;
	float window_height = 600 * sc_f;
	ExtendedRenderWindow window(sf::Vector2u(window_width, window_height), "Project AJJ");
	window.setFramerateLimit(140);

	//------------TextureAtlas test---------------
	TextureManager* tex_mag = ResourceLoader::loadResources();
	TextureAtlas* robot = tex_mag->getAtlas("Rogue");
	robot->createRegionGrid(10, 10);
	robot->assignTextureId(TEXTURE_ID::RUN, sf::Vector2u(0, 2), sf::Vector2u(9, 2));
	robot->assignTextureId(TEXTURE_ID::IDLE, sf::Vector2u(0, 5), sf::Vector2u(9, 5));
	robot->assignTextureId(TEXTURE_ID::ATTACK, sf::Vector2u(0, 3), sf::Vector2u(9, 3));
	window.setTextureManager(tex_mag);

	//--------------------------------------------

	Scene* test_scene = FirstScene::createScene();
	Camera scene_camera;
	CollisionDetection* col_det = new CollisionDetection(test_scene->getSceneObjects());
	test_scene->setCollisionDetection(col_det);
	test_scene->setCamera(&scene_camera);
	//scene_camera.setCameraZoom(2); //Zoom. ERROR thrown when zoom is higher, say 8-9
	window.setActiveScene(test_scene);
	sf::Clock clock;
	sf::Time time;

	//----------------------- Create Players & Lock View --------------------

	TestObject* player_1 = new TestObject(sf::Vector2f(600.0f, 0.0f), sf::Vector2f(100.0f, 100.0f));
	TestObject* player_2 = new TestObject(sf::Vector2f(500.0f, 100.0f), sf::Vector2f(100.0f, 100.0f));
	player_1->setTextureName("Rogue");
	player_2->setTextureName("Rogue");
	std::vector<Object*> player_vector = {player_1, player_2};

	player_1->setId(1337);
	player_2->setId(420);

	test_scene->addSceneObject(player_1);
	test_scene->addSceneObject(player_2);

	test_scene->getCamera()->lockOnObject(player_1);

	//------------------------ Add Controls To Players --------------------

	Controller contr;
	contr.setObject(player_1);
	contr.bindActionToKey(contr.getObject()->getActions()[0], sf::Keyboard::Key::Unknown); //IDLE
	contr.bindActionToKey(contr.getObject()->getActions()[1], sf::Keyboard::Key::W);
	contr.bindActionToKey(contr.getObject()->getActions()[2], sf::Keyboard::Key::S);
	contr.bindActionToKey(contr.getObject()->getActions()[3], sf::Keyboard::Key::A);
	contr.bindActionToKey(contr.getObject()->getActions()[4], sf::Keyboard::Key::D);
	contr.bindActionToKey(contr.getObject()->getActions()[5], sf::Keyboard::Key::LShift); //FUN-TEST ATTACK

	Controller contr_2;
	contr_2.setObject(player_2);
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[0], sf::Keyboard::Key::Unknown); //IDLE
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[1], sf::Keyboard::Key::Up);
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[2], sf::Keyboard::Key::Down);
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[3], sf::Keyboard::Key::Left);
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[4], sf::Keyboard::Key::Right);
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[5], sf::Keyboard::Key::RShift); //FUN-TEST ATTACK

	/*
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[6], { sf::Keyboard::Key::Up,  sf::Keyboard::Key::L }); //SPEED X2
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[7], { sf::Keyboard::Key::Down, sf::Keyboard::Key::L });
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[8], { sf::Keyboard::Key::Left, sf::Keyboard::Key::L });
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[9], { sf::Keyboard::Key::Right, sf::Keyboard::Key::L });
	*/

	TestMainControllerObject* main_contr_obj = new TestMainControllerObject();
	main_contr_obj->setId(-1);

	MainController main_contr;
	main_contr.setObject(main_contr_obj);
	main_contr.setScene(test_scene);
	main_contr.bindActionToKey(main_contr.getObject()->getActions()[0], sf::Keyboard::Key::Unknown);	//IDLE
	main_contr.bindActionToKey(main_contr.getObject()->getActions()[1], sf::Keyboard::Key::I);			//ZOOM IN
	main_contr.bindActionToKey(main_contr.getObject()->getActions()[2], sf::Keyboard::Key::O);			//ZOOM OUT
	test_scene->addSceneObject(main_contr.getObject());

	int i = 1;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
				{
					scene_camera.lockOnObject(player_vector[i % 2]);
					i++;
				}
			}
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		std::system("cls");		//clears the terminal. !!!APPARENTLY NOT SUPER, ONLY DEBUGGING PURPOSES!!!


		//------------DATA CONTR-------------
		std::cout << "\n-----CONTR 1----\n";
		contr.processUserInput();
		
		
		std::cout << "\n{ ";
		for (auto& active_action : contr.getActiveActions())
			std::cout << active_action->getActionName() << ", ";
		std::cout << "}\n";
		std::cout << contr.getObject()->getVelocity().x << " , " << contr.getObject()->getVelocity().y << std::endl;
		std::cout << "-----------------\n\n";
		//-----------------------------


		contr_2.processUserInput();
		main_contr.processUserInput();

		
		

		// clear the window with transparent(black) color
		window.clear(sf::Color::Transparent);

		// draw everything here...
		//time = clock.getElapsedTime();
		//test_scene->getSceneObjects()[8]->setWorldPosition(sf::Vector2f(300.0f + 50 * std::cos(time.asSeconds()), 300.0f + 50 * std::sin(time.asSeconds())));
		window.drawActiveScene();

		// end the current frame
		window.display();
	}
	delete player_1;
	delete player_2;
	delete main_contr_obj;
	delete col_det;
	delete tex_mag;
	return 0;
}