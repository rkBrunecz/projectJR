#include "PBE\System\In_Game_Clock.h"
#include <cstdio>
#include <stdlib.h>

namespace pb
{
	In_Game_Clock::In_Game_Clock(double scale, short currentHour, short currentMinutes, short totalHours, short morningHours, short afternoonHours, short eveningHours, short dawnHours) : HOURS_IN_DAY(totalHours), MORNING_HOURS(morningHours), AFTERNOON_HOURS(afternoonHours), EVENING_HOURS(eveningHours), DAWN_HOURS(dawnHours)
	{
		// Ensure the total number of hours in a day is equal to the number of hours given to morning, afternoon, and evening
		if (HOURS_IN_DAY != (MORNING_HOURS + AFTERNOON_HOURS + EVENING_HOURS + DAWN_HOURS))
		{
			printf("Total hours in a day must be equal to MORNING_HOURS + AFTERNOON_HOURS + EVENING_HOURS\n");
			exit(EXIT_FAILURE);
		}

		clock = new sf::Clock();

		this->hours = currentHour;
		this->minutes = currentMinutes;

		timeScale = 1 / scale;
	}

	In_Game_Clock::In_Game_Clock(const In_Game_Clock& c) : HOURS_IN_DAY(c.HOURS_IN_DAY), MORNING_HOURS(c.MORNING_HOURS), AFTERNOON_HOURS(c.AFTERNOON_HOURS), EVENING_HOURS(c.EVENING_HOURS), DAWN_HOURS(c.DAWN_HOURS)
	{
		// Allocate memory
		clock = new sf::Clock(); 
		clock = c.clock;

		hours = c.hours;
		minutes = c.minutes;

		timeScale = c.timeScale;
	}

	In_Game_Clock::~In_Game_Clock()
	{
		delete clock;
	}

	void In_Game_Clock::updateClock()
	{
		if (clock->getElapsedTime().asSeconds() > timeScale)
		{
			minutes += 1;
			clock->restart();
		}

		if (minutes == 60)
		{
			hours += 1;
			minutes = 0;

			if (hours == 24)
				hours = 0;
		}
	}

	void In_Game_Clock::pause()
	{
		delete clock;

		clock = 0; // Avoid double delete
	}

	void In_Game_Clock::resume()
	{
		clock = new sf::Clock();
	}

	const short In_Game_Clock::getMorningHours()
	{
		return MORNING_HOURS;
	}

	const short In_Game_Clock::getAfternoonHours()
	{
		return AFTERNOON_HOURS;
	}

	const short In_Game_Clock::getEveningHours()
	{
		return EVENING_HOURS;
	}
	
	const short In_Game_Clock::getDawnHours()
	{
		return DAWN_HOURS;
	}

	Time In_Game_Clock::getTime()
	{
		return pb::Time(hours, minutes, 0);
	}
}