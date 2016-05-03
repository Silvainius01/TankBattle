#pragma once
#include "Matrix3.h"
using namespace std;

namespace cml
{
	class Matrix4
	{
	public:
		float data[4][4];

		Matrix4(void);
		Matrix4(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4);

		Matrix4 getTranspose();
		Matrix4 getInverse();
		const Vec4 getRow(int row);
		void setRow(int row, Vec4 v);
		void print();
		Matrix4 &operator+(Matrix4 m);
		Matrix4 &operator+=(Matrix4 m);
		Matrix4 &operator-(Matrix4 m);
		Matrix4 &operator-=(Matrix4 m);
		Matrix4 &operator*(Matrix4 m);
		Matrix4 &operator*=(Matrix4 m);
		Matrix4 &operator=(Matrix4 m);
		Matrix4 &operator*(float f);
	};
}