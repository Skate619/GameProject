#ifndef THPC_MATHS_H
#define THPC_MATHS_H

#include <math.h>
#include "thpc_defines.h"


namespace thpc
{
	// Returns the minimum of two values
	float Min(float a, float b);
	// Returns the maximum of two values
	float Max(float a, float b);
	// Constrains the passed "value" to lie between "a" and "b"
	float Clamp(float value, float a=1.f, float b=0.f);
	
	class Vec2
	{
	public:
		Vec2();
		Vec2(float x, float y);
		Vec2(Vec2 &other);
		Vec2 operator+=(const Vec2 &other);
		Vec2 operator+(const Vec2 &other);
		Vec2 operator*(const Vec2 &other);

		float magnitude(const Vec2 &other);
	private:
		float x, y;
	};

	class Vec3
	{
	public:
		Vec3();
		Vec3(float x, float y, float z);
		Vec3(Vec3& other);
		Vec3 operator+=(const Vec3 &other);
		Vec3 operator+(const Vec3 &other);
		Vec3 operator*(const Vec3 &other);

		float magnitude(const Vec3 &other);
	private:
		float x, y, z;
	};
}


#endif