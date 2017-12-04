#include "thpc_time.h"
using namespace thpc;


Time::Time() 
	: s(0), 
	frac(0.f)
{}

Time::Time(int seconds) 
	: s(seconds), 
	frac(0.f)
{}

Time::Time(int seconds, float fraction) 
	: s(seconds),
	frac(fraction)
{}

Time::Time(float fraction) 
	: s(0),
	frac(fraction)
{}

float Time::_frac()
{
	return this->frac;
}

int Time::_s()
{
	return this->s;
}

void Time::elapse(int seconds)
{
	this->s += seconds;
}

void Time::elapse(float seconds)
{
	this->frac += seconds;
	correct();
}

void Time::elapse(Time by)
{
	this->frac += by._frac();
	this->s += by._s();
	correct();
}

void Time::reset()
{
	this->s = 0;
	this->frac = 0.f;
}

Time Time::operator=(int & seconds)
{
	this->s = seconds;
	frac = 0.f;
	return *this;
}

Time Time::operator=(float & seconds)
{
	this->s = 0;
	this->frac = seconds;
	correct();
	return *this;
}

Time Time::operator=(Time & other)
{
	this->s = other._s();
	this->frac = other._frac();
	correct();
	return *this;
}

bool Time::operator==(int &seconds)
{
	if(this->frac != 0.f)
		return false;
	if(this->s != seconds)
		return false;
	return true;
}

bool Time::operator==(float & seconds)
{
	float compare = this->frac;
	compare += (float)this->s;
	if(compare != seconds) 
		return false;
	return true;
}

bool Time::operator==(Time & other)
{
	if(this->s != other.s)
		return false;
	if(this->frac != other.frac)
		return false;
	return true;
}

Time Time::operator+=(int & seconds)
{
	elapse(seconds);
	return *this;
}

Time Time::operator+(int & seconds)
{
	Time result = {};
	result += seconds;
	return result;
}

Time Time::operator+=(float &seconds)
{
	elapse(seconds);
	return *this;
}

Time Time::operator+(float &seconds)
{
	Time result = {};
	result += seconds;
	return result;
}

Time Time::operator+=(Time &other)
{
	elapse(other);
	return *this;
}

Time Time::operator+(Time &other)
{
	Time result = {};
	result += other;
	return result;
}

Time Time::operator-=(int &seconds)
{
	elapse(-seconds);
	return *this;
}

Time Time::operator-(int &seconds)
{
	Time result = {};
	result -= seconds;
	return result;
}

Time Time::operator-=(float &seconds)
{
	elapse(-seconds);
	return *this;
}

Time Time::operator-(float &seconds)
{
	Time result = {};
	result -= seconds;
	return result;
}

Time Time::operator-=(Time &other)
{
	elapse(other);
	return *this;
}

Time Time::operator-(Time &other)
{
	Time result = {};
	result += other;
	return result;
}

int Time::getMillisecs(bool Constrain)
{
	int result = (int)(this->frac * 1000.f);
	if (!Constrain) 
		result += this->s;
	return result;
}

int Time::getSeconds(bool Constrain)
{
	int result = this->s;
	if (Constrain) 
		result = result % 60;
	return result;
}

int Time::getMinutes(bool Constrain)
{
	int result = this->s / 60;
	if (Constrain)
		result = result % 60;
	return result;
}

int Time::getHours(bool Constrain)
{
	int result = this->s / 3600;
	if (Constrain) 
		result = result % 24;
	return result;
}

int Time::getDays()
{
	return (this->s / 86400);
}

float Time::getMillisecsReal(bool Constrain)
{
	float result = this->frac * 1000.f;
	if (!Constrain) 
		result += (float)this->s *1000.f;
	return result;
}

float Time::getSecondsReal(bool Constrain)
{
	float result = this->frac;
	if (!Constrain) 
		result += (float)this->s;
	return result;
}

float Time::getMinutesReal(bool Constrain)
{
	float result = (float)this->s / 60.f;
	result += this->frac / 60.f;
	if (Constrain)
	{
		unsigned int trunc = this->s / 3600;
		result -= (float)(trunc * 60);
	}
	return result;
}

float Time::getHoursReal(bool Constrain)
{
	float result = (float)this->s / 3600.f;
	result += this->frac / 3600.f;
	if (Constrain)
	{
		unsigned int trunc = this->s / 86400;
		result -= (float)(trunc * 60);
	}
	return result;
}

float Time::getDaysReal()
{
	float result = (float)this->s / 86400.f;
	result += this->frac / 86400.f;
	return result;
}


void Time::correct()
{
	if (this->frac >= 1.0f)
	{
		int delta = (int)(this->frac);
		this->frac -= (float)(delta);
		this->s += delta;
	}
	else if(frac < 0.0f)
	{
		int delta = (int)(-this->frac);
		this->frac += (float)(delta);
		this->s -= delta;
	}
}
