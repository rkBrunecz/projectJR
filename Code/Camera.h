/*
Camera.h
This class inherits functions from the sf::View class and implements a few extra features.

@author Randall Brunecz
@version 1.0 9/11/2015
*/

#ifndef Camera_H
#define Camera_H

#include <SFML\Graphics.hpp>
#include "Audio_Engine.h"

class Camera 
{
public:
	// CAMERA STATES
	static const enum States
	{
		Overworld,
		Battle
	};

	// CAMERA METHODS
	static void initialize(int windowWidth, int windowHeight);
	static void setBounds(float x, float y);
	static void updatePosition(sf::Vector2f position);
	static void emphasisZoomIn(float x, float y, float zoomFactor);
	static void emphasisZoomOut();
	static void zoom(float zoomfactor);
	static void setSize(float x, float y);
	static void move(float x, float y);
	static void resetBattleCamera();
	static void setCenter(float x, float y);
	static void startCameraShake();
	static void animateCamera();
	static void setCameraState(States state);
	static sf::View getCamera();
	static bool cameraIsShaking();

private:
	// PRIVATE METHODS
	static void cameraShake();

	// CONSTANTS
	static const int NUM_DISPLACEMENTS = 10;

	// VARIABLES
	static sf::View camera;
	static sf::Vector2f cameraBounds;
	static float previousZoom, previousX, previousY, defaultSizeX, defaultSizeY, defaultBattleX, defaultBattleY;;
	static short shakeSoundId;
	static int displacementsPos;
	static sf::Clock clock;
	static sf::Vector2f displacements[NUM_DISPLACEMENTS];
	static bool shakeCamera;
	static States state;
};

#endif