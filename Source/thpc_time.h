#ifndef THPC_TIME_H
#define THPC_TIME_H

#include "thpc_defines.h"


namespace thpc
{
	class Time
	{
	public:
		// Constructors
		Time();
		Time(int seconds);
		Time(int seconds, float fraction);
		Time(float fraction);
		
		// Overloads
		Time operator=(int &seconds);
		Time operator=(float &seconds);
		Time operator=(Time &other);
		bool operator==(int &seconds);
		bool operator==(float &seconds);
		bool operator==(Time &other);
		Time operator+=(int &seconds);
		Time operator+(int &seconds);
		Time operator+=(float &seconds);
		Time operator+(float &seconds);
		Time operator+=(Time &other);
		Time operator+(Time &other);
		Time operator-=(int &seconds);
		Time operator-(int &seconds);
		Time operator-=(float &seconds);
		Time operator-(float &seconds);
		Time operator-=(Time &other);
		Time operator-(Time &other);

		// Time specific gets
		int getMillisecs(bool Constrain=true);
		int getSeconds(bool Constrain=true);
		int getMinutes(bool Constrain=true);
		int getHours(bool Constrain=true);
		int getDays();

		// Accurate time specific gets
		float getMillisecsReal(bool Constrain=true);
		float getSecondsReal(bool Constrain=true);
		float getMinutesReal(bool Constrain=true);
		float getHoursReal(bool Constrain=true);
		float getDaysReal();

		// Elapsed
		void elapse(int seconds);
		void elapse(float seconds);
		void elapse(Time by);

		void reset();

		// Value getters
		float _frac();
		int _s();

	private:
		int s;
		float frac;
		void correct();
	};
}


#endif