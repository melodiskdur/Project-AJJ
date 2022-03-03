#pragma once
#include "Button.h"

namespace TestButton
{
	Button* createTestButton(sf::Vector2f size, Action* action,bool on_fixated_layer, sf::String text);
}

