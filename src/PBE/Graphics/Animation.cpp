#include "PBE\Graphics\Animation.h"

namespace pb
{
	Animation::Animation(int numFrames, int startingLoopFrame, int finalLoopFrame, int width, int height, float updateInterval) : NUM_FRAMES(numFrames), STARTING_LOOP_FRAME(startingLoopFrame), FINAL_LOOP_FRAME(finalLoopFrame), WIDTH(width), HEIGHT(height), updateInterval(updateInterval)
	{

	}

	void Animation::newInterval(float interval)
	{
		updateInterval = interval;
	}
}