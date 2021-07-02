#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <string>
#include <iostream>
#include "TextureIds.h"

/*Animation
* An animation is connected to a specific action.
* If the jump action is used, the jump animation is played.
* An animation is a sequence of sprites
*/

struct Frame
{
	TEXTURE_ID texture_id;	//the sprite to be shown
	double duration;		//how long it will be shown
};

class Animation
{
public:
	Animation();
	~Animation();

	//Getters
	Frame getActiveFrame();

	//Setters
	void setActiveFrame(Frame frame);

	//Add a frame to the animation
	void addFrame(TEXTURE_ID texture_id, double duration);

	void update();

private:

	std::vector<Frame> frames; //a collection of frames
	int active_frame_index = 0;

	double total_progress = 0.0; //the current timestamp in the animation
	double total_length = 0.0; //the length of the whole animation

protected:

};
