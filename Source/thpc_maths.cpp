#include "thpc_maths.h"
using namespace thpc;


float thpc::Min(float a, float b)
{
	// If b is smaller than a, return b; else, return a
	return (b < a) ? b : a;
}

float thpc::Max(float a, float b)
{
	// If b is larger than a, return b; else, return a
	return (b > a) ? b : a;
}

float thpc::Clamp(float value, float a, float b)
{
	// Determine which is the high and the low of our two passed boundaries
	float high = b, low = a;
	if (low > high)
	{
		low = b; high = a;
	}
	// If value is smaller than low, return low;
	//  else if value is larger than high, return high;
	//  otherwise (i.e. if value is between low and high) return value
	return (value < low) ? low : ((value > high) ? high : value);
}


Vec2::Vec2()
	: x(0.f), y(0.f)
{}

Vec2::Vec2(float x, float y)
	: x(x), y(y)
{}

Vec2::Vec2(Vec2 &other)
	: x(other.x), y(other.y)
{}

Vec2 Vec2::operator+=(const Vec2 &other)
{
	this->x += other.x;
	this->y += other.y;
	return *this;
}

Vec2 Vec2::operator+(const Vec2 &other)
{
	Vec2 Result = *this;
	Result += other;
	return Result;
}

Vec2 Vec2::operator*(const Vec2 &other)
{
	Vec2 Result = Vec2();
	Result.x = this->x * other.x;
	Result.y = this->y * other.y;
	return Result;
}

float Vec2::magnitude(const Vec2 &other)
{
	float Result = sqrt((other.x * other.x) + (other.y * other.y));
	return Result;
}


Vec3::Vec3()
	: x(0.f), y(0.f), z(0.f)
{}

Vec3::Vec3(float x, float y, float z)
	: x(x), y(y), z(z)
{}

Vec3::Vec3(Vec3& other)
	: x(other.x), y(other.y), z(other.z)
{}

Vec3 Vec3::operator+=(const Vec3 &other)
{
	this->x += other.x;
	this->y += other.y;
	this->z += other.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3 &other)
{
	Vec3 Result = *this;
	Result += other;
	return Result;
}

Vec3 Vec3::operator*(const Vec3 &other)
{
	Vec3 Result = Vec3();
	Result.x = this->x * other.x;
	Result.y = this->y * other.y;
	Result.z = this->z * other.z;
	return Result;
}

float Vec3::magnitude(const Vec3 &other)
{
	float Result = sqrt((other.x * other.x) + (other.y * other.y) + (other.z * other.z));
	return Result;
}
