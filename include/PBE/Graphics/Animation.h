/*
Animation.h

This class defines some basic variables and methods for animations.

@author Randall Brunecz
@version 1.0 9/26/2016
*/

#ifndef Animation_H
#define Animation_H

namespace pb
{
	class Animation
	{
	public:
		void newInterval(float interval);

	protected:
		const int NUM_FRAMES, STARTING_LOOP_FRAME, FINAL_LOOP_FRAME, WIDTH, HEIGHT;
		float updateInterval;

		Animation(int numFrames, int startingLoopFrame, int finalLoopFrame, int width, int height, float updateInterval);
	};
}

#endif