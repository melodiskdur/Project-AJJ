#include "Project_AJJ.h"
#include "includes/TestObject.h"
#include "includes/FirstScene.h"
#include "includes/ResourceLoader.h"
#include "includes/TestMainControllerObject.h"
#include "includes/MainController.h"
#include "includes/TestLayout.h"

int main()
{
	sf::Event event;
	sf::Clock clock;

	//Test environment
	float sc_f = 1; //scale_factor for window width/height
	float window_width = 800 * sc_f;
	float window_height = 600 * sc_f;
	sf::ContextSettings settings;
	settings.antialiasingLevel = 10;
	ExtendedRenderWindow window(sf::Vector2u(window_width, window_height), "Project AJJ", settings);
	window.setFramerateLimit(30);

	//---------------------------- Layout test -------------------------------

	Layout* test_layout = TestLayout::createTestLayout();
	window.addLayout(test_layout);
	
	//------------------------- TextureAtlas test ----------------------------

	TextureManager* tex_mag = ResourceLoader::loadResources();
	TextureAtlas* robot = tex_mag->getAtlas("Rogue");
	robot->createRegionGrid(10, 10);
	robot->assignTextureId(TEXTURE_ID::RUN, sf::Vector2u(0, 2), sf::Vector2u(9, 2));
	robot->assignTextureId(TEXTURE_ID::IDLE, sf::Vector2u(0, 5), sf::Vector2u(9, 5));
	robot->assignTextureId(TEXTURE_ID::ATTACK, sf::Vector2u(0, 3), sf::Vector2u(9, 3));
	window.setTextureManager(tex_mag);

	//------------------- Scene , Camera, PhysicsManager, CollisionDetection, Hitbox, Gravity, AirFriction --------------------

	Scene* test_scene = FirstScene::createScene();
	Camera scene_camera;
	AirFriction* airfric = new AirFriction(test_scene->getSceneObjects());
	Gravity* gravity = new Gravity(test_scene->getSceneObjects());
	CollisionDetection* col_det = new CollisionDetection(test_scene->getSceneObjects());
	CollisionGraph* col_graph = new CollisionGraph();
	PhysicsManager* phys_mag = new PhysicsManager(test_scene->getSceneObjects());
	//Hitbox* hitbox = new Hitbox();
	phys_mag->addAttribute(col_det);
	phys_mag->addAttribute(gravity);
	phys_mag->addAttribute(airfric);
	phys_mag->col_graph = col_graph;
	test_scene->setCollisionDetection(col_det);
	test_scene->setPhysicsManager(phys_mag);
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

	// ---------------------- Some NPC Objects --------------------------
	TestObject* npc_1 = new TestObject(sf::Vector2f(700.f, 110.f), sf::Vector2f(100.f, 100.f), true);
	TestObject* npc_2 = new TestObject(sf::Vector2f(700.f, 10.f), sf::Vector2f(100.f, 100.f), true);
	TestObject* npc_3 = new TestObject(sf::Vector2f(1000.f, 110.f), sf::Vector2f(100.f, 100.f), true);
	TestObject* npc_4 = new TestObject(sf::Vector2f(1050.f, 10.f), sf::Vector2f(100.f, 100.f), true);
	TestObject* npc_5 = new TestObject(sf::Vector2f(1100.f, -90.f), sf::Vector2f(100.f, 100.f), true);
	TestObject* npc_6 = new TestObject(sf::Vector2f(1150.f, -190.f), sf::Vector2f(100.f, 100.f), true);

	TestObject* npc_7 = new TestObject(sf::Vector2f(1400.f, -320.f), sf::Vector2f(50.f, 430.f), true);
	TestObject* npc_8 = new TestObject(sf::Vector2f(1400.f, -370.f), sf::Vector2f(500.f, 50.f), true);

	TestObject* npc_9 = new TestObject(sf::Vector2f(1800.f, -470.f), sf::Vector2f(100.f, 100.f), true);

	npc_1->setId(1001);
	npc_2->setId(1002);
	npc_3->setId(1003);
	npc_4->setId(1004);
	npc_5->setId(1005);
	npc_6->setId(1006);
	npc_7->setId(1007);
	npc_8->setId(1008);
	npc_9->setId(1009);
	test_scene->addSceneObject(npc_1);
	test_scene->addSceneObject(npc_2);
	test_scene->addSceneObject(npc_3);
	test_scene->addSceneObject(npc_4);
	test_scene->addSceneObject(npc_5);
	test_scene->addSceneObject(npc_6);
	test_scene->addSceneObject(npc_7);
	test_scene->addSceneObject(npc_8);
	test_scene->addSceneObject(npc_9);

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
	main_contr.bindActionToKey(main_contr_obj->getActions()[4], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::P });			//PAUSE
	main_contr.bindActionToKey(main_contr_obj->getActions()[5], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::L });			//PLAY
	main_contr.bindActionToKey(main_contr_obj->getActions()[6], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::Down });		//MENU_MOVE_DOWN
	main_contr.bindActionToKey(main_contr_obj->getActions()[7], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::Up });		//MENU_MOVE_UP
	main_contr.bindActionToKey(main_contr_obj->getActions()[8], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::Enter });		//MENU_CHOOSE_ALTERNATIVE
	main_contr.bindActionToKey(main_contr_obj->getActions()[9], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::Delete });	//EXIT_TO_MENU
	//last, add the main_controller_object to the scene
	test_scene->addSceneObject(main_contr_obj);

	//---------------------------------- Game Loop -----------------------------------------
	
	while (window.isOpen())
	{
		
		//Start debugging
		sf::Time framerate = clock.getElapsedTime();
		clock.restart();
		//End debugging

		//for closing the window
		while (window.pollEvent(event))
		{
			/*
			if (event.type == sf::Event::KeyPressed)
			{
				
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
				{

				}
				
			}*/
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		main_contr.processUserInput();

		
		contr_player_1.processUserInput();
		contr_player_2.processUserInput();
		
		
		window.drawActiveScene();

		//DEBUGGING
		/*
		window.getLayouts()[0]->setPosition({ window.getLayouts()[0]->getPosition().x + 1 , window.getLayouts()[0]->getPosition().y + 1});
		window.getLayouts()[0]->setSize({ window.getLayouts()[0]->getSize().x + 2, window.getLayouts()[0]->getSize().y + 2});
		test_layout->resetMarginSpaces();
		test_layout->placeLayouts();
		*/
		//END DEBUGGING
		

		//Start debugging
		//prints out the framerate
		//std::cout << 1.0f / framerate.asSeconds() << std::endl;
		//End debugging
		
	}
	delete player_1;
	delete player_2;
	delete main_contr_obj;
	delete col_det;
	delete tex_mag;
	delete test_layout;
	return 0;
}