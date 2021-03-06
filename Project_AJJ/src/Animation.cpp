#include "Animation.h"

unsigned int Animation::instance_counter = 0;

sf::Clock Animation::animation_clock;

//Constructor
Animation::Animation()
{
	Animation::instance_counter++;
}

//Constructor
Animation::Animation(TEXTURE_ID texture_id, int start_index, int stop_index, double duration)
{
	Animation::instance_counter++;
	for (int i = start_index; i <= stop_index; i++)
	{
		addFrame(texture_id, i, duration);
	}
}

//Destructor
Animation::~Animation()
{
	Animation::instance_counter--;
}

//returns the active frame ie the frame currently displayed
Frame Animation::getActiveFrame()
{
	//if there are no frames
	if (this->frames.empty())
	{
		//return an empty frame
		Frame empty_frame;
		empty_frame.duration = 0;
		empty_frame.texture_id = TEXTURE_ID::NONE;
		return empty_frame;
	}

	//else, return the active frame
	return this->frames[this->active_frame_index];
}

//Adds a frame to the animation
void Animation::addFrame(TEXTURE_ID texture_id, double duration)
{
	//Create a new frame and add it to the animation sequence
	Frame new_frame;
	new_frame.texture_id = texture_id;
	new_frame.duration = duration;
	this->frames.push_back(new_frame);

	//Increase the total length of the animation by adding the frame duration
	this->total_length += duration;
}

void Animation::addFrame(TEXTURE_ID texture_id, int index, double duration)
{
	this->addFrame(texture_id, duration);
	this->frames.back().frame_index = index;
}

void Animation::addFrame(sf::String region_name, int index, double duration)
{
	this->addFrame(TEXTURE_ID::NONE, duration);
	this->frames.back().frame_index = index;
	this->frames.back().region_name = region_name;
}

//Sets the active frame index
void Animation::setActiveFrame(Frame frame)
{
	this->active_frame_index = frame.frame_index;
}

//Update the active frame index ie go to the next frame
void Animation::updateAnimation()
{
	total_progress = Animation::animation_clock.getElapsedTime().asMilliseconds();

	//if we aren't at the last frame
	if ( (this->active_frame_index + 1) < this->frames.size())
	{	
		//If the time difference between the total progress and the last frame update is greater than the frame duration.
		if (total_progress - last_update > frames[active_frame_index].duration)
		{
			//go to the next frame
			this->active_frame_index += 1;
			last_update = total_progress;
		}
	}
	else
	{
		//else, go back to the first frame ie restart the animation
		this->active_frame_index = 0;
	}
}
