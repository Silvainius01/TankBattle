#pragma once
#include "TemplateMath.h"
#include <iostream>
using namespace std;

namespace cml
{
	class Matrix3;

	struct Vec2
	{
		union
		{
			float data[2];
			struct { float x, y; };
		};

		Vec2() { x = 0, y = 0; }
		Vec2(float X, float Y) : x(X), y(Y) {}

		//Returns the magnitude
		float getMag();
		//Returns the angle relative to the global X-Axis
		float getAngle();
		//Returns the angle relative to another vector
		float getAngleBetween(Vec2 v2);
		//Calculates and returns the dot product with the passed vector
		float getDot(Vec2 v2);
		//Return the distance between two vectors
		float getDistBetween(Vec2 v2);
		//Returns the normal
		Vec2 getNormal();
		//Returns the perpendicular
		Vec2 getPerp();
		//Returns the reflection over the passed axis
		Vec2 getReflection(Vec2 axis);

		//Normalizes the vector
		void normalize();
		//Modify the vectors magnitude
		void lengthen(float mod);
		//Reflect the vector over a passed axis
		void reflect(Vec2 axis);
		//Print this vector to console
		void print();

		Vec2 operator+(Vec2 &v2);
		Vec2 &operator+=(Vec2 &v2);
		Vec2 operator-(Vec2 &v2);
		Vec2 &operator-();
		Vec2 &operator-=(Vec2 &v2);
		Vec2 operator*(Vec2 &v2);
		Vec2 &operator*=(Vec2 &v2);
		Vec2 operator/(Vec2 &v2);
		Vec2 &operator/=(Vec2 &v2);
		Vec2 &operator=(const Vec2 &v2);
		Vec2 operator*(float a);
		Vec2 &operator*=(float a);
		Vec2 operator/(const float a);
		Vec2 &operator/=(float a);
		Vec2 operator-(float a);
		Vec2 &operator-=(float a);

		Vec2 operator*(const Matrix3 &m);
		Vec2 &operator*=(const Matrix3 &m);
		
		bool operator==(Vec2 v2);
		bool operator!=(Vec2 v2);
		bool operator>=(Vec2 v2);
		bool operator<=(Vec2 v2);
		bool operator>(Vec2 v2);
		bool operator<(Vec2 v2);
		bool operator==(float a);
		bool operator!=(float a);
		bool operator>=(float a);
		bool operator<=(float a);
		bool operator>(float a);
		bool operator<(float a);
		bool operator==(int a);
		bool operator!=(int a);
		bool operator>=(int a);
		bool operator<=(int a);
		bool operator>(int a);
		bool operator<(int a);
		
		float &operator[](int a);
	};

	//Return { | x | , | y | }
	Vec2 abs(const Vec2 &v2);
}