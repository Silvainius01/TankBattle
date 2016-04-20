#pragma once
#include "TemplateMath.h"
#include <iostream>
using namespace std;

namespace cml
{
	class Matrix4;

	class Vec4
	{
	public:
		union
		{
			float data[4];
			struct { float x, y, z, w; };
		};

		Vec4() { x = 0, y = 0, z = 0, w = 0; }
		Vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

		//Returns the magnitude of this vector
		float getMag();
		//Returns the dot product using the passed vector
		float getDot(Vec4 v4);
		//returns the normal of this vector
		Vec4 getNormal();
		//Returns the reflection of this vector
		Vec4 getReflection(Vec4 axis);

		//Normalizes this vector
		void normalize();
		//Modify this vecotrs magnitude
		void lengthen(float mod);
		//Reflect this vector over the passed axis
		void reflect(Vec4 axis);
		//Print this vector to console
		void print();

		Vec4 &operator+(Vec4 v4);
		Vec4 &operator+=(Vec4 v4);
		Vec4 &operator-(Vec4 v4);
		Vec4 &operator-();
		Vec4 &operator-=(Vec4 v4);
		Vec4 &operator*(Vec4 v4);
		Vec4 &operator*=(Vec4 v4);
		Vec4 &operator/(Vec4 v4);
		Vec4 &operator/=(Vec4 v4);
		Vec4 &operator=(Vec4 v4);
		
		Vec4 &operator*(float a);
		Vec4 &operator*=(float a);
		Vec4 &operator/(float a);
		Vec4 &operator/=(float a);

		Vec4 &operator*(Matrix4 &m);
		Vec4 &operator*=(Matrix4 &m);
		
		bool operator==(Vec4 v2);
		bool operator!=(Vec4 v2);
		bool operator>=(Vec4 v2);
		bool operator<=(Vec4 v2);
		bool operator>(Vec4 v2);
		bool operator<(Vec4 v2);
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
}