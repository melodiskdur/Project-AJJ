#include "pch.h"
#include "CppUnitTest.h"
#include "Object.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ProjectAJJUnitTests
{
	TEST_CLASS(Object_UnitTest)
	{
	public:
		TEST_METHOD(Test_ObjectInit_Pos)
		{
			Object* test_object = new Object(sf::Vector2f(20.f, 20.f), sf::Vector2f(100.f, 100.f));
			sf::Vector2f expected_pos = { 20.f, 20.f };
			sf::Vector2f actual_pos = test_object->getWorldPosition();
			float TOL = 0.001f;
			Assert::AreEqual(expected_pos.x, actual_pos.x, TOL);
			Assert::AreEqual(expected_pos.y, actual_pos.y, TOL);
			delete test_object;
		}
		TEST_METHOD(Test_ObjectInit_Size)
		{
			Object* test_object = new Object(sf::Vector2f(20.f, 20.f), sf::Vector2f(139.f, 134.5f));
			sf::Vector2f expected_size = { 139.f, 134.5f };
			sf::Vector2f actual_size = test_object->getSize();
			float TOL = 0.001f;
			Assert::AreEqual(expected_size.x, actual_size.x, TOL);
			Assert::AreEqual(expected_size.y, actual_size.y, TOL);
			delete test_object;
		}
	};
}
