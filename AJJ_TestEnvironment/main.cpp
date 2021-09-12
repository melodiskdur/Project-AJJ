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
	window.setFramerateLimit(60);

	//------------------------- TextureAtlas test ----------------------------

	TextureManager* tex_mag = ResourceLoader::loadResources();
	TextureAtlas* robot = tex_mag->getAtlas("Rogue");
	robot->createRegionGrid(10, 10);
	robot->assignTextureId(TEXTURE_ID::RUN, sf::Vector2u(0, 2), sf::Vector2u(9, 2));
	robot->assignTextureId(TEXTURE_ID::IDLE, sf::Vector2u(0, 5), sf::Vector2u(9, 5));
	robot->assignTextureId(TEXTURE_ID::ATTACK, sf::Vector2u(0, 3), sf::Vector2u(9, 3));
	window.setTextureManager(tex_mag);

	//------------------- Scene , Camera, CollisionDetection --------------------

	Scene* test_scene = FirstScene::createScene();
	Camera scene_camera;
	CollisionDetection* col_det = new CollisionDetection(test_scene->getSceneObjects());
	test_scene->setCollisionDetection(col_det);
	test_scene->setCamera(&scene_camera);
	window.setActiveScene(test_scene);
	//sf::Clock clock;
	//sf::Time time;

	//----------------------- Create Players & Lock View --------------------

	TestObject* player_1 = new TestObject(sf::Vector2f(600.0f, 0.0f), sf::Vector2f(100.0f, 100.0f));
	TestObject* player_2 = new TestObject(sf::Vector2f(500.0f, 100.0f), sf::Vector2f(100.0f, 100.0f));
	player_1->setTextureName("Rogue");
	player_2->setTextureName("Rogue");
	player_1->setId(1337);
	player_2->setId(420);
	test_scene->addSceneObject(player_1);
	test_scene->addSceneObject(player_2);
	test_scene->getCamera()->lockOnObject(player_1);

	//------------------------ Add Controls To Players --------------------

	Controller contr_player_1;
	contr_player_1.setObject(player_1);
	contr_player_1.bindActionToKey(player_1->getActions()[0], sf::Keyboard::Key::Unknown); //IDLE
	contr_player_1.bindActionToKey(player_1->getActions()[1], sf::Keyboard::Key::W);
	contr_player_1.bindActionToKey(player_1->getActions()[2], sf::Keyboard::Key::S);
	contr_player_1.bindActionToKey(player_1->getActions()[3], sf::Keyboard::Key::A);
	contr_player_1.bindActionToKey(player_1->getActions()[4], sf::Keyboard::Key::D);
	contr_player_1.bindActionToKey(player_1->getActions()[5], sf::Keyboard::Key::LShift); //FUN-TEST ATTACK

	Controller contr_player_2;
	contr_player_2.setObject(player_2);
	contr_player_2.bindActionToKey(player_2->getActions()[0], sf::Keyboard::Key::Unknown); //IDLE
	contr_player_2.bindActionToKey(player_2->getActions()[1], sf::Keyboard::Key::Up);
	contr_player_2.bindActionToKey(player_2->getActions()[2], sf::Keyboard::Key::Down);
	contr_player_2.bindActionToKey(player_2->getActions()[3], sf::Keyboard::Key::Left);
	contr_player_2.bindActionToKey(player_2->getActions()[4], sf::Keyboard::Key::Right);
	contr_player_2.bindActionToKey(player_2->getActions()[5], sf::Keyboard::Key::RShift); //FUN-TEST ATTACK

	/*
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[6], { sf::Keyboard::Key::Up,  sf::Keyboard::Key::L }); //SPEED X2
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[7], { sf::Keyboard::Key::Down, sf::Keyboard::Key::L });
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[8], { sf::Keyboard::Key::Left, sf::Keyboard::Key::L });
	contr_2.bindActionToKey(contr_2.getObject()->getActions()[9], { sf::Keyboard::Key::Right, sf::Keyboard::Key::L });
	*/

	//--------------------------- Add Main Controller --------------------------------
	
	//create the main controller
	MainController main_contr;
	//create object
	TestMainControllerObject* main_contr_obj = new TestMainControllerObject();
	//set to -1 (reserved for the main controller)
	main_contr_obj->setId(-1);
	//set the object to the main controller
	main_contr.setObject(main_contr_obj);
	//set the window for the main controller
	main_contr.setWindow(&window);
	//add controllers 
	main_contr.addController(&contr_player_1);
	main_contr.addController(&contr_player_2);
	//bind actions to specific keys
	main_contr.bindActionToKey(main_contr_obj->getActions()[0], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::Unknown });	//IDLE
	main_contr.bindActionToKey(main_contr_obj->getActions()[1], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::I });			//ZOOM IN
	main_contr.bindActionToKey(main_contr_obj->getActions()[2], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::O });			//ZOOM OUT
	main_contr.bindActionToKey(main_contr_obj->getActions()[3], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::Space });		//SWITCH PLAYERS TEST
	main_contr.bindActionToKey(main_contr_obj->getActions()[4], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::P });	//PAUSE
	main_contr.bindActionToKey(main_contr_obj->getActions()[5], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::L }); //PLAY
	//last, add the main_controller_object to the scene
	test_scene->addSceneObject(main_contr_obj);

	//---------------------------------- Game Loop -----------------------------------------

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				/*
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
				{

				}
				*/
			}
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}
    
		main_contr.processUserInput();
		std::cout << "\n{{ ";
		for (auto& active_action : main_contr.getActiveActions())
			std::cout << active_action->getActionName() << ", ";
		std::cout << "}}\n";
    
		/////////////////////////////////////////////
		//------------DATA CONTR_PLAYER_1-------------
		//std::system("cls");		//clears the terminal
    
		std::cout << "\n-----CONTR 1----\n";

		contr_player_1.processUserInput();
		
		std::cout << "\n{ ";
		for (auto& active_action : contr_player_1.getActiveActions())
			std::cout << active_action->getActionName() << ", ";
		std::cout << "}\n";

		std::cout << contr_player_1.getObject()->getVelocity().x << 
			" , " << contr_player_1.getObject()->getVelocity().y << std::endl;

		std::cout << "-----------------\n\n";
		//-------------------------------------------
		//////////////////////////////////////////////

		contr_player_2.processUserInput();
		
		//ADDED WINDOW.CLEAR AND WINDOW.DISPLAY IN drawActiveScene()!!!!!!!!!!!!!!!!!!!!

		// draw everything here...
		//time = clock.getElapsedTime();
		//test_scene->getSceneObjects()[8]->setWorldPosition(sf::Vector2f(300.0f + 50 * std::cos(time.asSeconds()), 300.0f + 50 * std::sin(time.asSeconds())));
		
		window.drawActiveScene();
	}
	delete player_1;
	delete player_2;
	delete main_contr_obj;
	delete col_det;
	delete tex_mag;
	return 0;
}