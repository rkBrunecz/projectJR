/*
Battle animations are animation that occur in cycles used in battles. Things like moving towards a point that animate a sprites cloak or something similar during the movement. 


*/

#include "SFML\Graphics.hpp"

#ifndef Battle_Animation_H
#define Battle_Animation_H

class Battle_Animation
{
public:
	//PUBLIC FUNCTIONS
	void intializeAnimationData(short numFrames, short frameDelay, short finalFrameDelay, short initialFrameDelay, short startLoopFrame, short finalLoopFrame, 
		int height, int width, int top, int defaultLeft, bool loopOnce);
	void animate(sf::Sprite* sprite, sf::Clock* clock, bool reset);
	void reverseAnimation();
	bool onLoopFrame(sf::Sprite* sprite);
	bool onFirstFrame(sf::Sprite* sprite, sf::Clock* clock);
	bool isReversing();
	bool isAnimating(sf::Sprite* sprite);
	void changeFrameDelay(short delay);
	void changeInitialFrameDelay(short delay);
	void changeFinalFrameDelay(short delay);

private:
	//PRIVATE FUNCTIONS
	void nextFrame(sf::Sprite* sprite, sf::Clock* clock);
	void resetAnimation(sf::Sprite* sprite, sf::Clock* clock);

	//PRIVATE CONSTANTS
	const enum States {
		Forward,
		Backward
	}; 

	//PRIVATE VARIABLES
	short numFrames = 0; //Number of frames in the attack
	short frameDelay = 0; //Amount of time to elapse before switching to next frame in an attack
	short finalFrameDelay = 0;
	short initialFrameDelay = 0;
	short startLoopFrame = 0;
	short finalLoopFrame = 0;
	int height = 0;
	int width = 0;
	int top = 0; //top of the row that the animation frames are in
	int defaultLeft = 0; //Left side of the frame of the first part of the animation
	int left = 0;
	int counter = 0;
	States state = Forward;
	bool reverse = false;
	bool loopOnce = false;
};

#endif