/*
Camera.h
This class inherits functions from the sf::View class and implements a few extra features.

@author Randall Brunecz
@version 1.0 9/11/2015
*/

#ifndef Camera_H
#define Camera_H

#include <SFML\Graphics.hpp>
#include "PBE\Audio\Audio.h" 

namespace pb
{
	class Camera : public sf::View
	{
	public:
		// Constructors
		Camera(int windowWidth, int windowHeight, float zoomFactor);
		Camera(int windowWidth, int windowHeight, float zoomFactor, const std::string& shakeSoundFileName);

		// CAMERA METHODS
		void setBounds(float x, float y);
		void updatePosition(const sf::Vector2f& position);
		void setCameraSize(float x, float y);
		void moveCameraInBounds();
		const sf::Vector2f getCameraOrigins();
		const sf::Vector2f getCameraBounds();
		
		// ANIMATION METHODS
		void animateCamera();
		void startCameraShake(const float factor);
		bool cameraIsShaking();
		void emphasisZoomIn(float x, float y, float zoomFactor);
		void emphasisZoomOut();

	private:
		// PRIVATE METHODS
		void cameraShake();

		// CONSTANTS
		const enum Animations
		{
			Shake,
			EmphasisZoomIn,
			EmphasisZoomOut,
			None
		};
		const int NUM_DISPLACEMENTS = 10;

		// VARIABLES
		sf::Vector2f cameraBounds, zoomTo, cameraOrigins;
		std::vector<sf::Vector2f> displacements;
		sf::Clock clock;
		Animations animation = None;
		float previousZoom, zoomFactor, previousX = 0, previousY = 0, defaultSizeX = 0, defaultSizeY = 0;
		short shakeSoundId = -1;
		int displacementsPos = 0;
	};
}
#endif