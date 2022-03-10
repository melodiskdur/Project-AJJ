#include "Project_AJJ.h"
#include "includes/TestObject.h"
#include "includes/FirstScene.h"
#include "includes/ResourceLoader.h"
#include "includes/TestMainControllerObject.h"
#include "includes/MainController.h"
#include "includes/TestLayout.h"

void instanceCounts(std::string s);

int main()
{
	sf::Event event;
	sf::Clock clock;

	//Test environment
	int sc_f = 1; //scale_factor for window width/height
	unsigned int window_width = static_cast<unsigned int>(800 * sc_f);
	unsigned int window_height = static_cast<unsigned int>(600 * sc_f);
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;
	ExtendedRenderWindow window(sf::Vector2u(window_width, window_height), "Project AJJ", settings);
	window.setFramerateLimit(30);
	window.setVerticalSyncEnabled(true);

	//---------------------------- Layout test -------------------------------

	//Layout* test_layout = TestLayout::createTestLayout();
	//window.addLayout(test_layout);

	//------------------------- TextureAtlas test ----------------------------

	TextureManager* tex_mag = ResourceLoader::loadResources();
	TextureAtlas* robot = tex_mag->getAtlas("Rogue");
	robot->createRegionGrid(10, 10);
	robot->assignTextureId(TEXTURE_ID::RUN, sf::Vector2u(0, 2), sf::Vector2u(9, 2));
	robot->assignTextureId(TEXTURE_ID::IDLE, sf::Vector2u(0, 5), sf::Vector2u(9, 5));
	robot->assignTextureId(TEXTURE_ID::ATTACK, sf::Vector2u(0, 3), sf::Vector2u(9, 3));
	window.setTextureManager(tex_mag);

	//------------------- Scene , Camera, PhysicsManager, CollisionDetection, CollisionHandler, Gravity, AirFriction --------------------

	Scene* test_scene = FirstScene::createScene();
	Camera scene_camera;
	AirFriction* airfric = new AirFriction(test_scene->getSceneObjects());
	Gravity* gravity = new Gravity(test_scene->getSceneObjects());
	CollisionDetection* col_det = new CollisionDetection(test_scene->getSceneObjects());
	CollisionGraph* col_graph = new CollisionGraph();
	PhysicsManager* phys_mag = new PhysicsManager(test_scene->getSceneObjects());
	//CollisionHandler* CollisionHandler = new CollisionHandler();
	phys_mag->addAttribute(col_det);
	phys_mag->addAttribute(gravity);
	phys_mag->addAttribute(airfric);
	phys_mag->col_graph = col_graph;
	test_scene->setCollisionDetection(col_det);
	test_scene->setPhysicsManager(phys_mag);
	test_scene->setCamera(&scene_camera);
	window.setActiveScene(test_scene);

	//----------------------- Create Players & Lock View --------------------

	TestObject* player_1 = new TestObject(sf::Vector2f(1800.0f, 0.0f), sf::Vector2f(100.0f, 100.0f));
	TestObject* player_2 = new TestObject(sf::Vector2f(400.0f, 100.0f), sf::Vector2f(100.0f, 100.0f));
	player_1->setTextureName("Rogue");
	player_2->setTextureName("Rogue");
	player_1->setId(1337);
	player_2->setId(420);
	test_scene->addSceneObject(player_1);
	test_scene->addSceneObject(player_2);
	test_scene->getCamera()->lockOnObject(player_1);

	// ------------------------ Create "NPCs" ----------------------------
	TestObject* npc1 = new TestObject(sf::Vector2f(650.f, 0.f), sf::Vector2f(200.f, 200.f), true);
	TestObject* npc2 = new TestObject(sf::Vector2f(650.f, 0.f), sf::Vector2f(200.f, 200.f), true);
	npc1->setId(0001);
	npc2->setId(0002);
	test_scene->addSceneObject(npc1);
	test_scene->addSceneObject(npc2);
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
	//main_contr.bindActionToKey(main_contr_obj->getActions()[8], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::Enter });	//MENU_CHOOSE_ALTERNATIVE
	//main_contr.bindActionToKey(main_contr_obj->getActions()[9], { sf::Keyboard::Key::LControl, sf::Keyboard::Key::Delete });	//EXIT_TO_MENU
	
	//bind a button to the left mouse button

	//DEBUGGING

	int in_world_layout = -2;
	for (auto& l : test_scene->getLayer(in_world_layout)->getLayouts()[0]->getLayouts())
	{
		for (auto obj : l->getObjects())
		{
			main_contr.bindActionToMouseButton(obj->getActions()[0], obj, sf::Mouse::Button::Left);		//LAYOUT BUTTON TEST
		}
	}

	int static_layout = -3;
	for (auto& l : test_scene->getLayer(static_layout)->getLayouts()[0]->getLayouts())
	{
		for (auto obj : l->getObjects())
		{
			main_contr.bindActionToMouseButton(obj->getActions()[0], obj, sf::Mouse::Button::Left);		//LAYOUT BUTTON TEST
		}
	}

	//END DEBUGGING

	//last, add the main_controller_object to the scene
	test_scene->addSceneObject(main_contr_obj);
	
	instanceCounts(" created");

	// Call to store Hitboxes.
	col_det->findHitboxes();

	//---------------------------------- Game Loop -----------------------------------------
	
	while (window.isOpen())
	{
		
		//DEBUGGING
		//sf::Time framerate = clock.getElapsedTime();
		//clock.restart();
		//END DEBUGGING

		//for closing the window
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}	
		}

		//take and process the input for the main_controller as well as all of the connected controllers 
		main_contr.processUserInput();
		
		//draw the active_scene
		window.drawActiveSceneToWindow();

		//DEBUGGING
		//window.getLayouts()[0]->setPositionForAll({ window.getLayouts()[0]->getPosition().x + 1 , window.getLayouts()[0]->getPosition().y + 1});
		//window.getLayouts()[0]->setSize({ window.getLayouts()[0]->getSize().x + 2, window.getLayouts()[0]->getSize().y + 2});
		//window.getLayouts()[0]->moveForAll({ 1,0 });
		//test_layout->resetMarginSpaces();
		//test_layout->placeLayouts();
		//prints out the framerate
		//std::cout << 1.0f / framerate.asSeconds() << std::endl;
		//END DEBUGGING	
	}
	delete test_scene;
	delete phys_mag;
	delete col_det;
	delete col_graph;
	delete airfric;
	delete gravity;
	//delete test_layout;
	delete tex_mag;
	window.~ExtendedRenderWindow();
	scene_camera.~Camera();
	contr_player_1.~Controller();
	contr_player_2.~Controller();
	main_contr.~MainController();

	instanceCounts(" remaining");

	return 0;
}

void instanceCounts(std::string s)
{
	std::cout << "Actions" << s << ": " << Action::instanceCount() << "\n";
	std::cout << "Animations" << s << ": " << Animation::instanceCount() << "\n";
	std::cout << "Cameras" << s << ": "<< Camera::instanceCount() << "\n";
	std::cout << "Controllers" << s << ": " << Controller::instanceCount() << "\n";
	std::cout << "ExtendedRenderWindows" << s << ": " << ExtendedRenderWindow::instanceCount() << "\n";
	std::cout << "Layouts" << s << ": " << Layout::instanceCount() << "\n";
	std::cout << "Objects" << s << ": " << Object::instanceCount() << "\n";
	std::cout << "PhysicsAttributes" << s << ": " << PhysicsAttribute::instanceCount() << "\n";
	std::cout << "Scene" << s << ": " << Scene::instanceCount() << "\n";
	std::cout << "SceneLayer" << s << ": " << SceneLayer::instanceCount() << "\n";
	std::cout << "TextureAtlases" << s << ": " << TextureAtlas::instanceCount() << "\n";
	std::cout << "TextureManagers" << s << ": " << TextureManager::instanceCount() << "\n";
	std::cout << "PropertyNodes" << s << ": " << PropertyNode::instanceCount() << "\n";
	std::cout << "\n========================================================\n";
}