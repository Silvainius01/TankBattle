#pragma once
#include "TemplateMath.h"
#include "Vec4.h"
#include "Vec3.h"
#include "Vec2.h"
#include <iostream>
using namespace std;

namespace cml
{
	class Matrix3
	{
	public:
		float data[3][3];

		Matrix3(void);
		Matrix3(Vec3 v1, Vec3 v2, Vec3 v3);

		//Returns the transpose of the matrix
		Matrix3 getTranspose();
		//Retunr sthe invers eof the matirx
		Matrix3 getInverse();
		//Return the determinant of this Matrix
		float getDeterminant();
		//Returns the passed row as a Vec3
		Vec3 getRow(int row);
		//Sets the passed row with the passed Vec3
		void setRow(int row, Vec3 v3);
		//Print this Matrix to the console.
		void print();

		Matrix3 &operator+(const Matrix3 &m);
		Matrix3 &operator+=(const Matrix3 &m);
		Matrix3 &operator-(const Matrix3 &m);
		Matrix3 &operator-=(const Matrix3 &m);
		Matrix3 &operator*(const Matrix3 &m);
		Matrix3 &operator*=(const Matrix3 &m);
		Matrix3 &operator=(const Matrix3 &m);
		Matrix3 &operator*(const float &f);
		Matrix3 &operator*=(const float &f);
		bool operator==(const Matrix3 &m)
		{
			for(int a = 0; a < 3; a++)
			{
				for (int b = 0; a < 3; a++)
				{
					if (data[a][b] != m.data[a][b]) { return false; }
				}
			}
			return true;
		}
	};
}