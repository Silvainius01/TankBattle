#pragma once
#include "TemplateMath.h"
#include <iostream>
using namespace std;

namespace cml
{
	class Matrix3;

	struct Vec3
	{
		union
		{
			float data[3];
			struct { float x, y, z; };
		};

		Vec3() { x = 0, y = 0, z = 0; }
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		//Return the vector's magnitude
		float getMag();
		//Return the dot product using the passed vector
		float getDot(Vec3 v3);
		//Returns the normal of this vector
		Vec3 getNormal();
		//Retunrs the corss product using the passed vector
		Vec3 getCross(Vec3 v3);
		//Returns the reflection over the passed axis
		Vec3 getReflection(Vec3 axis);

		//Normalize this vector
		void normalize();
		//Modify this vecotr's magnitude
		void lengthen(float mod);
		//Reflect this vecotr over the passed axis
		void reflect(Vec3 axis);
		//print this vecotr to console
		void print();

		Vec3 &operator+(Vec3 v3);
		Vec3 &operator+=(Vec3 v3);
		Vec3 &operator-(Vec3 v3);
		Vec3 &operator-();
		Vec3 &operator-=(Vec3 v3);
		Vec3 &operator*(Vec3 v3);
		Vec3 &operator*=(Vec3 v3);
		Vec3 &operator/(Vec3 v3);
		Vec3 &operator/=(Vec3 v3);
		Vec3 &operator=(Vec3 v3);
		Vec3 &operator*(float a);
		Vec3 &operator*=(float a);
		Vec3 &operator/(float a);
		Vec3 &operator/=(float a);
		Vec3 &operator*(const Matrix3 &m);
		Vec3 &operator*=(const Matrix3 &m);


		bool operator==(Vec3 v2);
		bool operator!=(Vec3 v2);
		bool operator>=(Vec3 v2);
		bool operator<=(Vec3 v2);
		bool operator>(Vec3 v2);
		bool operator<(Vec3 v2);
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