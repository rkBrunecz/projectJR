#ifndef In_Game_Clock_H
#define In_Game_Clock_H

#include "SFML\System\Clock.hpp"
#include "SFML\System\Vector3.hpp"
#include "PBE\System\Time.h"

namespace pb
{
	class In_Game_Clock
	{
	public:
		// Constructor

		/**
		In_Game_Clock
		
		When determing hours, make sure you keep in mind that evening hours start at 00:00, morning hours start at > EVENING HOURS, and
		afternoon hours start at > EVENING HOURS + MORNING HOURS.

		Parameters:
			scale: Scale the time by this number. Will increase, or decrease, the passage of time
			currentHour: The current hour in game
			currentMinutes: The current in game minutes
			totalHours: The total hours in a day
			morningHours: Total number of morning hours. Morning start after Dawn ends.
			afternoonHours: Total number of afternoon hours. Afternoon start after morning ends.
			eveningHours: Total number of evening hours. Evening hours start after afternoon ends.
			dawnHours: Total number of dawn hours. Dawn hours start after evening ends.
		*/
		In_Game_Clock(double scale, short currentHour, short currentMinutes, short totalHours, short morningHours, short afternoonHours, short eveningHours, short dawnHours);

		// Copy Constructor
		In_Game_Clock(const In_Game_Clock& c);

		// Destructor
		~In_Game_Clock();

		void updateClock();
		void pause();
		void resume();

		void changeFactor(double timeScale);

		const short getMorningHours();
		const short getAfternoonHours();
		const short getEveningHours();
		const short getDawnHours();
		Time getTime();

	private:
		// Operators
		const In_Game_Clock &operator=(const In_Game_Clock& c) { return *this; };

		const short HOURS_IN_DAY;
		const short MORNING_HOURS, AFTERNOON_HOURS, EVENING_HOURS, DAWN_HOURS;

		short hours, minutes;
		double timeScale;
		sf::Clock *clock = 0;
	};
}

#endif