#include "PBE\Graphics\Day_Shift_Animation.h"
#include <math.h>
#include <cstdio>

namespace pb
{
	Day_Shift_Animation::Day_Shift_Animation(short shadowAlphaMax, In_Game_Clock& clock) : SHADOW_ALPHA_MAX(shadowAlphaMax), MORNING_HOURS(clock.getMorningHours()), AFTERNOON_HOURS(clock.getAfternoonHours()), EVENING_HOURS(clock.getEveningHours()), DAWN_HOURS(clock.getDawnHours())
	{
		// Local variables
		sf::Color colorOfDay;
		short totalMinutesPassed;
		pb::Time time = clock.getTime();

		// Determine the time of day
		if (time.hours >= EVENING_HOURS + DAWN_HOURS + MORNING_HOURS) // Afternoon
		{
			timeOfDay = Afternoon;
			determineIntervals(AFTERNOON, EVENING, AFTERNOON_HOURS);
			colorOfDay = AFTERNOON;

			// Resolve to a negative number to calculate the correct color of day (this is because the color value should be decrementing downwards to color value EVENING)
			totalMinutesPassed = -(time.hours - (EVENING_HOURS + DAWN_HOURS + MORNING_HOURS)) * 60; // 60 minutes in an hour
			shadowAlpha = SHADOW_ALPHA_MAX - (totalMinutesPassed / shadowUpdateInterval);
		}
		else if (time.hours >= EVENING_HOURS + DAWN_HOURS) // Morning
		{
			timeOfDay = Morning;
			determineIntervals(MORNING, AFTERNOON, MORNING_HOURS);
			colorOfDay = MORNING;

			// Resolve to a negative number to calculate the correct color of day (this is because the color value should be decrementing downwards to color value EVENING)
			totalMinutesPassed = -(time.hours - (EVENING_HOURS + DAWN_HOURS)) * 60; // 60 minutes in an hour
		}
		else if (time.hours >= EVENING_HOURS) // Dawn
		{
			timeOfDay = Dawn;
			determineIntervals(DAWN, MORNING, EVENING_HOURS);
			colorOfDay = DAWN;

			// Resolve to a positive number to calculate the correct color of day (this is because the color value should be incrementing upwards to color value MORNING)
			totalMinutesPassed = time.hours * 60; // 60 minutes in an hour
			shadowAlpha = SHADOW_ALPHA_MAX + (totalMinutesPassed / shadowUpdateInterval);
		}
		else // Evening
		{
			timeOfDay = Evening;
			determineIntervals(EVENING, DAWN, EVENING_HOURS);
			colorOfDay = EVENING;

			// Resolve to a positive number to calculate the correct color of day (this is because the color value should be incrementing upwards to color value MORNING)
			totalMinutesPassed = time.hours * 60; // 60 minutes in an hour
			shadowAlpha = 0.0f;
		}

		// Calculate the correct color value of the given time
		dayTimeColor.r = (sf::Uint8)(colorOfDay.r + (totalMinutesPassed * redUpdateInterval));
		dayTimeColor.g = (sf::Uint8)(colorOfDay.g + (totalMinutesPassed * greenUpdateInterval));
		dayTimeColor.b = (sf::Uint8)(colorOfDay.b + (totalMinutesPassed * greenUpdateInterval));

		accumulator = sf::Vector3f(dayTimeColor.r, dayTimeColor.g, dayTimeColor.b);
	}

	void Day_Shift_Animation::determineIntervals(sf::Color dayColor, sf::Color shiftToColor, short timeOfDayHours)
	{
		redUpdateInterval = (float)abs(dayColor.r - shiftToColor.r) / (timeOfDayHours * MINUTES_IN_HOUR);
		greenUpdateInterval = (float)abs(dayColor.g - shiftToColor.g) / (timeOfDayHours * MINUTES_IN_HOUR);
		blueUpdateInterval = (float)abs(dayColor.b - shiftToColor.b) / (timeOfDayHours * MINUTES_IN_HOUR);

		if (dayColor == AFTERNOON)
			shadowUpdateInterval = (float)SHADOW_ALPHA_MAX / ((timeOfDayHours - 2) * MINUTES_IN_HOUR); // subtract 2 to have shadows disappear slightly faster before it becomes too dark
		else if (dayColor == DAWN)
			shadowUpdateInterval = (float)SHADOW_ALPHA_MAX / (timeOfDayHours * MINUTES_IN_HOUR);
	}

	const sf::Color Day_Shift_Animation::updateDayTime(Time& time)
	{
		// Check to see if the colors have already been updated
		if (lastUpdate.minutes == time.minutes)
			return dayTimeColor;
		
		// Update last update to reflect the new time
		lastUpdate = time;

		switch (timeOfDay)
		{
		case Morning:
			accumulator = sf::Vector3f(accumulator.x - redUpdateInterval, accumulator.y - greenUpdateInterval, accumulator.z - blueUpdateInterval);
			
			// Calculate the correct color value of the given time
			if (dayTimeColor.r > AFTERNOON.r && dayTimeColor.g > AFTERNOON.g && dayTimeColor.b > AFTERNOON.b)
				dayTimeColor = sf::Color((int)accumulator.x, (int)accumulator.y, (int)accumulator.z);

			if ((dayTimeColor.r <= AFTERNOON.r || dayTimeColor.g <= AFTERNOON.g || dayTimeColor.b <= AFTERNOON.b) && MORNING_HOURS + AFTERNOON_HOURS == time.hours)
			{
				timeOfDay = Afternoon;
				dayTimeColor = AFTERNOON;
				accumulator = sf::Vector3f(AFTERNOON.r, AFTERNOON.g, AFTERNOON.b);

				determineIntervals(AFTERNOON, EVENING, AFTERNOON_HOURS);
			}

			break;

		case Afternoon:
			accumulator = sf::Vector3f(accumulator.x - redUpdateInterval, accumulator.y - greenUpdateInterval, accumulator.z - blueUpdateInterval);

			// Calculate the correct color value of the given time
			if (dayTimeColor.r > EVENING.r && dayTimeColor.g > EVENING.g && dayTimeColor.b > EVENING.b)
				dayTimeColor = sf::Color((int)accumulator.x, (int)accumulator.y, (int)accumulator.z);

			if (shadowAlpha > 1)
				shadowAlpha -= shadowUpdateInterval;
			else
				shadowAlpha = 0;

			if ((dayTimeColor.r <= EVENING.r || dayTimeColor.g <= EVENING.g || dayTimeColor.b <= EVENING.b) && shadowAlpha == 0 && time.hours == 0)
			{
				timeOfDay = Evening;
				dayTimeColor = EVENING;
				accumulator = sf::Vector3f(EVENING.r, EVENING.g, EVENING.b);

				determineIntervals(EVENING, DAWN, EVENING_HOURS);
			}

			break;

		case Evening:
			accumulator = sf::Vector3f(accumulator.x + redUpdateInterval, accumulator.y + greenUpdateInterval, accumulator.z + blueUpdateInterval);

			// Calculate the correct color value of the given time
			if (dayTimeColor.r < DAWN.r && dayTimeColor.g < DAWN.g && dayTimeColor.b < DAWN.b)
				dayTimeColor = sf::Color((int)accumulator.x, (int)accumulator.y, (int)accumulator.z);


			if ((dayTimeColor.r >= DAWN.r || dayTimeColor.g >= DAWN.g || dayTimeColor.b >= DAWN.b) && time.hours == DAWN_HOURS)
			{
				timeOfDay = Dawn;
				dayTimeColor = DAWN;
				accumulator = sf::Vector3f(DAWN.r, DAWN.g, DAWN.b);

				determineIntervals(DAWN, MORNING, DAWN_HOURS);
			}

			break;

		case Dawn:
			accumulator = sf::Vector3f(accumulator.x + redUpdateInterval, accumulator.y + greenUpdateInterval, accumulator.z + blueUpdateInterval);

			// Calculate the correct color value of the given time
			if (dayTimeColor.r < MORNING.r && dayTimeColor.g < MORNING.g && dayTimeColor.b < MORNING.b)
				dayTimeColor = sf::Color((int)accumulator.x, (int)accumulator.y, (int)accumulator.z);

			// Calculate shadow alpha
			if (shadowAlpha < (float)SHADOW_ALPHA_MAX)
				shadowAlpha += shadowUpdateInterval;
			else
				shadowAlpha = (float)SHADOW_ALPHA_MAX;

			if ((dayTimeColor.r >= MORNING.r || dayTimeColor.g >= MORNING.g || dayTimeColor.b >= MORNING.b) && shadowAlpha == SHADOW_ALPHA_MAX && time.hours == MORNING_HOURS)
			{
				timeOfDay = Morning;
				dayTimeColor = MORNING;
				accumulator = sf::Vector3f(MORNING.r, MORNING.g, MORNING.b);

				determineIntervals(MORNING, AFTERNOON, MORNING_HOURS);
			}

			break;
		}

		return dayTimeColor;
	}

	const short Day_Shift_Animation::getShadowAlpha()
	{
		return (short)shadowAlpha;
	}
}