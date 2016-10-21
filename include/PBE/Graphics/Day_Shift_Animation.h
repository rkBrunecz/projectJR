/*
Day_Shift_Animation.h

This class creates and updates a sf::Color object to represent the different colors of the day. It is designed to work with 
the class Time (defined in Time.h).

@author Randall Brunecz
@version 1.0 9/22/2016
*/

#ifndef Day_Shift_Animation_H
#define Day_Shift_Animation_H

// Headers
#include "SFML\Graphics\Color.hpp"
#include "SFML\System\Clock.hpp"
#include "SFML\System\NonCopyable.hpp"
#include "PBE\System\In_Game_Clock.h"

namespace pb
{
	class Day_Shift_Animation : sf::NonCopyable
	{
	public:
		// Constructor
		Day_Shift_Animation(short shadowAlphaMax, In_Game_Clock& clock);

		const sf::Color updateDayTime(Time& time);
		const short getShadowAlpha();
		const short getShadowAlphaMax() { return SHADOW_ALPHA_MAX; }

	private:
		// Private methods
		void determineIntervals(sf::Color dayColor, sf::Color shiftToColor, short timeOfDayHours);

		// CONSTANTS
		const sf::Color MORNING = sf::Color(255, 255, 255, 255),
			DAWN = sf::Color(130, 130, 130, 255),
			AFTERNOON = sf::Color(200, 130, 130, 255),
			EVENING = sf::Color(80, 80, 100, 255);
		const short SHADOW_ALPHA_MAX;
		const short MORNING_HOURS, AFTERNOON_HOURS, EVENING_HOURS, DAWN_HOURS;
		const short MINUTES_IN_HOUR = 60;

		// Enums
		const enum TimeOfDay
		{
			Morning,
			Afternoon,
			Evening,
			Dawn
		};

		// Private variables
		TimeOfDay timeOfDay = Morning;
		Time lastUpdate;
		sf::Color dayTimeColor;
		sf::Vector3f accumulator;
		float shadowAlpha = (float)SHADOW_ALPHA_MAX;
		float redUpdateInterval, greenUpdateInterval, blueUpdateInterval, shadowUpdateInterval;
	};
}

#endif