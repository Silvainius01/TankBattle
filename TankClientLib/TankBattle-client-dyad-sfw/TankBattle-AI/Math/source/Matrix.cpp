#include "../Matrix4.h"
using namespace cml;

Matrix3::Matrix3(void)
{
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			if (a == b) { data[a][b] = 1; }
			else { data[a][b] = 0; }
		}
	}
}
Matrix3::Matrix3(Vec3 v1, Vec3 v2, Vec3 v3) { setRow(0, v1); setRow(1, v2); setRow(2, v3); }
Matrix3 Matrix3::getTranspose()
{
	Matrix3 transpose = *this;

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			if (a > b)
			{
				float save = transpose.data[b][a];
				transpose.data[b][a] = transpose.data[a][b];
				transpose.data[a][b] = save;
			}
		}
	}

	return transpose;
}
float Matrix3::getDeterminant()
{
	return  ((data[0][0] * ((data[1][1] * data[2][2]) - (data[1][2] * data[2][1]))) -
		(data[1][0] * ((data[0][1] * data[2][2]) - (data[0][2] * data[2][1]))) +
		(data[2][0] * ((data[0][1] * data[1][2]) - (data[0][2] * data[1][1]))));
}
Matrix3 Matrix3::getInverse()
{
	Matrix3 adjugate;

	int count = 0;
	float e[4];

	for (int col = 0, row = 0; row < 3; col++)
	{
		for (int a = 0; a < 3; a++)
		{
			for (int b = 0; b < 3; b++)
			{
				if (a != col && b != row)
				{
					e[count] = this->data[a][b];
					count++;
				}
			}
		}

		adjugate.data[col][row] = (e[0] * e[3]) - (e[1] * e[2]);

		count = 0;

		if (col == 2)
		{
			col = -1;
			row++;
		}
	}

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			if (a - 1 == b)
			{
				adjugate.data[a][b] = -adjugate.data[a][b];
				adjugate.data[b][a] = -adjugate.data[b][a];
			}
		}
	}

	adjugate = adjugate.getTranspose();

	return adjugate * (1 / getDeterminant());
}
Vec3 Matrix3::getRow(int row) { return{ data[0][row], data[1][row], data[2][row] }; }
void Matrix3::setRow(int row, Vec3 v3) { for (int a = 0; a < 3; a++) { data[a][row] = v3[a]; } }
void Matrix3::print()
{

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			cout << data[b][a] << " ";
		}

		cout << endl;
	}
}
Matrix3 &Matrix3::operator+(const Matrix3 &m)
{
	Matrix3 blerg = *this;
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			blerg.data[a][b] += m.data[a][b];
		}
	}
	return blerg;
}
Matrix3 &Matrix3::operator+=(const Matrix3 &m)
{
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			data[a][b] += m.data[a][b];
		}
	}
	return *this;
}
Matrix3 &Matrix3::operator-(const Matrix3 &m)
{
	Matrix3 blerg = *this;
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			data[a][b] - m.data[a][b];
		}
	}
	return blerg;
}
Matrix3 &Matrix3::operator-=(const Matrix3 &m)
{
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			data[a][b] -= m.data[a][b];
		}
	}
	return *this;
}
Matrix3 &Matrix3::operator*(const Matrix3 &m)
{
	Vec3 rowData;
	Vec3 colData;

	Matrix3 blerg;

	for (int a = 0; a < 3; a++)
	{
		//Gets Column data from first
		for (int c = 0; c < 3; c++) { colData[c] = data[a][c]; }

		for (int b = 0; b < 3; b++)
		{
			//Gets row data from second
			for (int c = 0; c < 3; c++) { rowData[c] = m.data[c][b]; }

			blerg.data[a][b] = rowData.getDot(colData);
		}
	}

	return blerg;
}
Matrix3 &Matrix3::operator*=(const Matrix3 &m)
{
	Vec3 rowData;
	Vec3 colData;

	for (int a = 0; a < 3; a++)
	{
		//Gets Column data from first
		for (int c = 0; c < 3; c++) { colData[c] = data[a][c]; }

		for (int b = 0; b < 3; b++)
		{
			//Gets row data from second
			for (int c = 0; c < 3; c++) { rowData[c] = m.data[c][b]; }

			data[a][b] = rowData.getDot(colData);
		}
	}

	return *this;
}
Matrix3 &Matrix3::operator=(const Matrix3 &m)
{
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			data[a][b] = m.data[a][b];
		}
	}

	return *this;
}
Matrix3 &Matrix3::operator*(const float &f)
{
	Matrix3 blerg = *this;

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			blerg.data[a][b] *= f;
		}
	}

	return blerg;
}
Matrix3 &Matrix3::operator*=(const float &f)
{
	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++)
		{
			data[a][b] *= f;
		}
	}

	return *this;
}

Matrix4::Matrix4(void)
{
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			if (a == b) { data[a][b] = 1; }
			else { data[a][b] = 0; }
		}
	}
}
Matrix4::Matrix4(Vec4 v1, Vec4 v2, Vec4 v3, Vec4 v4)
{
	Vec4 blerg[4] = { v1, v2, v3, v4 };
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			data[a][b] = blerg[a][b];
		}
	}
}
Matrix4 Matrix4::getTranspose()
{
	Matrix4 transpose = *this;

	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			if (a > b)
			{
				float save = transpose.data[b][a];
				transpose.data[b][a] = transpose.data[a][b];
				transpose.data[a][b] = save;
			}
		}
	}

	return transpose;
}
Matrix4 Matrix4::getInverse()
{
	cout << "Error: Function not implemented; identity matrix returned.\n";
	return Matrix4();
}
const Vec4 Matrix4::getRow(int row)
{
	return{ data[0][row], data[1][row], data[2][row], data[3][row] };
}
void Matrix4::setRow(int row, Vec4 v)
{
	for (int a = 0; a < 4; a++) { data[a][row] = v[a]; }
}
void Matrix4::print()
{
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			cout << data[a][b] << " ";
		}
		cout << endl;
	}
}
Matrix4 &Matrix4::operator+(Matrix4 m)
{
	Matrix4 blerg;
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			blerg.data[a][b] += m.data[a][b];
		}
	}
	return blerg;
}
Matrix4 &Matrix4::operator+=(Matrix4 m)
{
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			data[a][b] += m.data[a][b];
		}
	}
	return *this;
}
Matrix4 &Matrix4::operator-(Matrix4 m)
{
	Matrix4 blerg;
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			blerg.data[a][b] - m.data[a][b];
		}
	}
	return blerg;
}
Matrix4 &Matrix4::operator-=(Matrix4 m)
{
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			data[a][b] -= m.data[a][b];
		}
	}
	return *this;
}
Matrix4 &Matrix4::operator*(Matrix4 m)
{
	Vec3 rowData;
	Vec3 colData;

	Matrix4 blerg;

	for (int a = 0; a < 4; a++)
	{
		//Gets Column data from first
		for (int c = 0; c < 4; c++) { colData[c] = data[a][c]; }

		for (int b = 0; b < 4; b++)
		{
			//Gets row data from second
			for (int c = 0; c < 4; c++) { rowData[c] = m.data[c][b]; }

			blerg.data[a][b] = rowData.getDot(colData);
		}
	}

	return blerg;
}
Matrix4 &Matrix4::operator*=(Matrix4 m)
{
	Vec4 rowData;
	Vec4 colData;

	for (int a = 0; a < 4; a++)
	{
		//Gets Column data from first
		for (int c = 0; c < 4; c++) { colData[c] = data[a][c]; }

		for (int b = 0; b < 4; b++)
		{
			//Gets row data from second
			for (int c = 0; c < 4; c++) { rowData[c] = m.data[c][b]; }

			data[a][b] = rowData.getDot(colData);
		}
	}

	return *this;
}
Matrix4 &Matrix4::operator=(Matrix4 m)
{
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			data[a][b] = m.data[a][b];
		}
	}

	return *this;
}
Matrix4 &Matrix4::operator*(float f)
{
	Matrix4 blerg;
	for (int a = 0; a < 4; a++)
	{
		for (int b = 0; b < 4; b++)
		{
			blerg.data[a][b] *= f;
		}
	}
	return blerg;
}