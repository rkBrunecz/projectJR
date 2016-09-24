/*
Time.h

This class is designed simply to be a convient way to contain time in hours, minutes, and seconds. It allows for code to be much 
more readable when using time to perform calculations. The class is meant to only be a container, and the data within should never 
need to be modified.

@author Randall Brunecz
@version 1.0 9/22/2016
*/

#ifndef Time_H
#define Time_H

namespace pb
{
	class Time
	{
	public:
		short hours, minutes, seconds;

		// Constructor
		Time() : hours(0), minutes(0), seconds(0) {};
		Time(short hours, short minutes, short seconds) : hours(hours), minutes(minutes), seconds(seconds) {};

		// Operators
		friend bool operator==(const Time& t1, const Time& t2)
		{
			if (t1.hours == t2.hours && t1.minutes == t2.minutes && t1.seconds == t2.seconds)
				return true;

			return false;
		}
	};
}

#endif