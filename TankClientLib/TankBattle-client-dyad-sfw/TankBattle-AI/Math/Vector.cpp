#include "Matrix4.h"
using namespace cml;

float Vec2::getMag() { return pythag(x, y); }
float Vec2::getAngle() { return atan2(x, y) * RAD2DEG; }
float Vec2::getAngleBetween(Vec2 v2) 
{ 
	if (x < v2.x) { return (atan((y - v2.y) / (x - v2.x)) * RAD2DEG) + 180; }
	return atan((y - v2.y) / (x - v2.x)) * RAD2DEG;
}
float Vec2::getDot(Vec2 v2)
{
	Vec2 blerg = { x, y }; blerg *= v2;
	return blerg.x + blerg.y;
}
float Vec2::getDistBetween(Vec2 v2)
{
	return (v2 - *this).getMag();
}
Vec2 Vec2::getNormal()
{
	return{ x / getMag(), y / getMag() };
}
Vec2 Vec2::getPerp() { return{ y, -x }; }
Vec2 Vec2::getReflection(Vec2 axis)
{
	Vec2 reflect = { x, y }; reflect -= (axis.getNormal() * (2 * this->getDot(axis.getNormal())));
	return reflect;
}
void Vec2::normalize() { x /= getMag(), y /= getMag(); }
void Vec2::lengthen(float mod)
{
	float mag = getMag();
	this->normalize();
	x *= mag + mod;
	y *= mag + mod;
	*this = Vec2{ x * (mag + mod), y * (mag + mod) };
}
void Vec2::reflect(Vec2 axis)
{
	Vec2 reflect = { x, y };
	reflect -= (axis.getNormal() * (2 * this->getDot(axis.getNormal())));
	x = reflect.x, y = reflect.y;
}
void Vec2::print() { cout << "( "; for (int a = 0; a < 2; a++) { cout << data[a] << " "; } cout << ") "; }
Vec2 Vec2::operator+(Vec2 &v2) 
{
	//Vec2{ x + v2.x, y + v2.y }.print();
	return Vec2{x + v2.x, y + v2.y};
}
Vec2 &Vec2::operator+=(Vec2 &v2) { x += v2.x; y += v2.y; return *this; }
Vec2 Vec2::operator-(Vec2 &v2) {
	Vec2 blerg = Vec2{ x - v2.x, y - v2.y };
	return blerg;
}
Vec2 &Vec2::operator-() { x = -x, y = -y; return *this; }
Vec2 &Vec2::operator-=(Vec2 &v2) { x -= v2.x; y -= v2.y; return *this; }
Vec2 Vec2::operator*(Vec2 &v2) 
{
	Vec2 blerg = Vec2{ x * v2.x, y*v2.y };
	return blerg;
}
Vec2 &Vec2::operator*=(Vec2 &v2) { x *= v2.x; y *= v2.y; return *this; }
Vec2 Vec2::operator/(Vec2 &v2) 
{
	Vec2 blerg = Vec2{ x / v2.x, y / v2.y };
	return blerg;
}
Vec2 &Vec2::operator/=(Vec2 &v2) { x /= v2.x; y /= v2.y; return *this; }
Vec2 &Vec2::operator=(const Vec2 &v2)
{ 
	this->x = v2.x;
	this->y = v2.y; 
	return *this; 
}
Vec2 Vec2::operator*(float a)
{
	return Vec2{ x * a, y*a };
}
Vec2 &Vec2::operator*=(float a) { x *= a; y *= a; return *this; }
Vec2 Vec2::operator/(const float a)
{
	return Vec2{ x / a, y / a };
}
Vec2 &Vec2::operator/=(float a) { x /= a; y /= a; return *this; }
Vec2 Vec2::operator-(float a) { return Vec2{ x - a, y - a }; }
Vec2 &Vec2::operator-=(float a) { x -= a; y -= a; return *this; }
Vec2 Vec2::operator*(const Matrix3 &m)
{
	Vec2 result;
	Vec3 blerg = Vec3{ x, y, 1 };
	blerg *= m;
	result = Vec2{ blerg.x, blerg.y };
	return result;
}
Vec2 &Vec2::operator*=(const Matrix3 &m)
{
	Vec3 blerg = Vec3{ x, y, 1 };
	blerg *= m;
	x = blerg.x, y = blerg.y;
	return *this;
}
bool Vec2::operator==(Vec2 v2) { return x == v2.x && y == v2.y; }
bool Vec2::operator!=(Vec2 v2) { return x != v2.x && y != v2.y; }
bool Vec2::operator>=(Vec2 v2) { return getMag() >= v2.getMag(); }
bool Vec2::operator<=(Vec2 v2) { return getMag() <= v2.getMag(); }
bool Vec2::operator>(Vec2 v2) { return getMag() > v2.getMag(); }
bool Vec2::operator<(Vec2 v2) { return getMag() < v2.getMag(); }
bool Vec2::operator==(float a) { return getMag() == a; }
bool Vec2::operator!=(float a) { return getMag() != a; }
bool Vec2::operator>=(float a) { return getMag() >= a; }
bool Vec2::operator<=(float a) { return getMag() <= a; }
bool Vec2::operator>(float a) { return getMag() > a; }
bool Vec2::operator<(float a) { return getMag() < a; }
bool Vec2::operator==(int a) { return getMag() == a; }
bool Vec2::operator!=(int a) { return getMag() != a; }
bool Vec2::operator>=(int a) { return getMag() >= a; }
bool Vec2::operator<=(int a) { return getMag() <= a; }
bool Vec2::operator>(int a) { return getMag() > a; }
bool Vec2::operator<(int a) { return getMag() < a; }
float &Vec2::operator[](int a) { return data[a]; }
Vec2 cml::abs(const Vec2 & v2)
{
	return Vec2{ std::abs(v2.x), std::abs(v2.y) };
}

float Vec3::getMag() { return sqrt(expo(x, 2.0f) + expo(y, 2.0f) + expo(z, 2.0f)); }
float Vec3::getDot(Vec3 v3)
{
	return ((x * v3.x) + (y * v3.y) + (z * v3.z));
}
Vec3 Vec3::getReflection(Vec3 axis)
{
	Vec3 reflect = { x, y, z };
	return reflect -= (axis.getNormal() * (2 * this->getDot(axis.getNormal())));
}
Vec3 Vec3::getNormal() { return{ x / getMag(), y / getMag(), z / getMag() }; }
Vec3 Vec3::getCross(Vec3 v3) { return{ (y * v3.z) - (z * v3.y), (z * v3.x) - (x * v3.z), (x * v3.y) - (y * v3.x) }; }
void Vec3::normalize() { x /= getMag(), y /= getMag(), z /= getMag(); }
void Vec3::lengthen(float mod)
{
	float mag = this->getMag();
	this->normalize();
	x *= mag + mod;
	y += mag + mod;
	z *= mag + mod;
}
void Vec3::reflect(Vec3 axis)
{
	Vec3 reflect = *this;
	reflect -= (axis.getNormal() * (2 * this->getDot(axis.getNormal())));
	x = reflect.x, y = reflect.y, z = reflect.z;
}
void Vec3::print() { cout << "( "; for (int a = 0; a < 3; a++) { cout << data[a] << " "; } cout << ")"; }
Vec3 &Vec3::operator+(Vec3 v3) { x += v3.x; y += v3.y; z += v3.z; return *this; }
Vec3 &Vec3::operator+=(Vec3 v3) { x += v3.x; y += v3.y; z += v3.z; return *this; }
Vec3 &Vec3::operator-(Vec3 v3) { x -= v3.x; y -= v3.y; z -= v3.z; return *this; }
Vec3 &Vec3::operator-() { x = -x, y = -y; z = -z; return *this; }
Vec3 &Vec3::operator-=(Vec3 v3) { x -= v3.x; y -= v3.y; z -= v3.z; return *this; }
Vec3 &Vec3::operator*(Vec3 v3) { x *= v3.x; y *= v3.y; z *= v3.z; return *this; }
Vec3 &Vec3::operator*=(Vec3 v3) { x *= v3.x; y *= v3.y; z *= v3.z; return *this; }
Vec3 &Vec3::operator/(Vec3 v3) { x /= v3.x; y /= v3.y; z /= v3.z; return *this; }
Vec3 &Vec3::operator/=(Vec3 v3) { x /= v3.x; y /= v3.y; z /= v3.z; return *this; }
Vec3 &Vec3::operator*(float a) { x *= a; y *= a; z *= a; return *this; }
Vec3 &Vec3::operator*=(float a) { x *= a; y *= a; z *= a; return *this; }
Vec3 &Vec3::operator/(float a) { x /= a; y /= a; z /= a; return *this; }
Vec3 &Vec3::operator/=(float a) { x /= a; y /= a; z /= a; return *this; }
Vec3 &Vec3::operator=(Vec3 v3) { x = v3.x, y = v3.y, z = v3.z; return *this; }
Vec3 &Vec3::operator*(const Matrix3 &m)
{
	Vec3 rowData;
	Vec3 colData;

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++) { rowData[b] = m.data[b][a]; }
		colData.data[a] = rowData.getDot(Vec3{x, y, z});
	}

	return colData;
}
Vec3 &Vec3::operator*=(const Matrix3 &m)
{
	Vec3 rowData;
	Vec3 colData = Vec3{x, y, z};

	for (int a = 0; a < 3; a++)
	{
		for (int b = 0; b < 3; b++) { rowData[b] = m.data[b][a]; }
		colData.data[a] = rowData.getDot(Vec3{x, y, z});
	}

	x = colData.x, y = colData.y, z = colData.z;
	return *this;
}
bool Vec3::operator==(Vec3 v2) { return x == v2.x && y == v2.y && z == v2.z; }
bool Vec3::operator!=(Vec3 v2) { return x != v2.x && y != v2.y && z != v2.z; }
bool Vec3::operator>=(Vec3 v2) { return getMag() >= v2.getMag(); }
bool Vec3::operator<=(Vec3 v2) { return getMag() <= v2.getMag(); }
bool Vec3::operator>(Vec3 v2) { return getMag() > v2.getMag(); }
bool Vec3::operator<(Vec3 v2) { return getMag() < v2.getMag(); }
bool Vec3::operator==(float a) { return getMag() == a; }
bool Vec3::operator!=(float a) { return getMag() != a; }
bool Vec3::operator>=(float a) { return getMag() >= a; }
bool Vec3::operator<=(float a) { return getMag() <= a; }
bool Vec3::operator>(float a) { return getMag() > a; }
bool Vec3::operator<(float a) { return getMag() < a; }
bool Vec3::operator==(int a) { return getMag() == a; }
bool Vec3::operator!=(int a) { return getMag() != a; }
bool Vec3::operator>=(int a) { return getMag() >= a; }
bool Vec3::operator<=(int a) { return getMag() <= a; }
bool Vec3::operator>(int a) { return getMag() > a; }
bool Vec3::operator<(int a) { return getMag() < a; }
float &Vec3::operator[](int a) { return data[a]; }

float Vec4::getDot(Vec4 v4)
{
	Vec4 blerg = { x, y, z, w }; blerg *= v4;
	return blerg.x + blerg.y + blerg.z + blerg.w;
}
float Vec4::getMag() { return sqrt(expo(x, 2.0f) + expo(y, 2.0f) + expo(z, 2.0f) + expo(w, 2.0f)); }
Vec4 Vec4::getNormal() { return{ x / getMag(), y / getMag(), z / getMag(), w / getMag() }; }
Vec4 Vec4::getReflection(Vec4 axis)
{
	Vec4 reflect = { x, y, z, w };
	return reflect -= (axis.getNormal() * (2 * this->getDot(axis.getNormal())));
}
void Vec4::normalize() { x /= getMag(), y /= getMag(), z /= getMag(), w /= getMag(); }
void Vec4::lengthen(float mod)
{
	float mag = this->getMag();
	this->normalize();
	x *= mag + mod, y *= mag + mod, z *= mag + mod, w *= mag + mod;
}
void Vec4::reflect(Vec4 axis)
{
	Vec4 reflect = { x, y, z, w };
	reflect -= (axis.getNormal() * (2 * this->getDot(axis.getNormal())));
	x = reflect.x, y = reflect.y, z = reflect.z, w = reflect.w;
}
void Vec4::print() { for (int a = 0; a < 4; a++) { cout << data[a] << " "; } }
Vec4 &Vec4::operator+(Vec4 v4) 
{
	Vec4 blerg = { x + v4.x, y + v4.y, z + v4.z, w + v4.w };
	return blerg;
}
Vec4 &Vec4::operator+=(Vec4 v4) { x += v4.x; y += v4.y; z += v4.z; w += v4.w; return *this; }
Vec4 &Vec4::operator-(Vec4 v4) 
{
	Vec4 blerg = { x - v4.x, y - v4.y, z - v4.z, w - v4.w };
	return blerg;
}
Vec4 &Vec4::operator-() { x = -x, y = -y; z = -z; w = -w; return *this; }
Vec4 &Vec4::operator-=(Vec4 v4) { x -= v4.x; y -= v4.y; z -= v4.z; w -= v4.w; return *this; }
Vec4 &Vec4::operator*(Vec4 v4) 
{
	Vec4 blerg = { x * v4.x, y * v4.y, z * v4.z, w * v4.w };
	return blerg;
}
Vec4 &Vec4::operator*=(Vec4 v4) { x *= v4.x; y *= v4.y; z *= v4.z; w *= v4.w; return *this; }
Vec4 &Vec4::operator/(Vec4 v4) 
{
	Vec4 blerg = { x / v4.x, y / v4.y, z / v4.z, w / v4.w };
	return blerg;
}
Vec4 &Vec4::operator/=(Vec4 v4) { x /= v4.x; y /= v4.y; z /= v4.z; w /= v4.w; return *this; }
Vec4 &Vec4::operator=(Vec4 v4) { x = v4.x; y = v4.y; z = v4.z; w = v4.w; return *this; }
Vec4 &Vec4::operator*(float a)
{
	Vec4 blerg = { x * a, y * a, z * a, w * a };
	return blerg;
}
Vec4 &Vec4::operator*=(float a) { x *= a; y *= a; z *= a; w *= a; return *this; }
Vec4 &Vec4::operator/(float a) 
{
	Vec4 blerg = { x / a, y / a, z / a, w / a };
	return blerg;
}
Vec4 &Vec4::operator/=(float a) { x /= a; y /= a; z /= a; w /= a; return *this; }
Vec4 &Vec4::operator*(Matrix4 &m)
{
	Vec4 blerg;
	for (int a = 0; a < 4; a++)
	{
		blerg[a] = this->getDot(m.getRow(a));
	}
	return blerg;
}
Vec4 &Vec4::operator*=(Matrix4 &m)
{
	for (int a = 0; a < 4; a++)
	{
		data[a] = this->getDot(m.getRow(a));
	}
	return *this;
}
bool Vec4::operator==(Vec4 v2) { return x == v2.x && y == v2.y && z == v2.z && w == v2.w; }
bool Vec4::operator!=(Vec4 v2) { return x != v2.x && y != v2.y && z != v2.z && w != v2.w; }
bool Vec4::operator>=(Vec4 v2) { return getMag() >= v2.getMag(); }
bool Vec4::operator<=(Vec4 v2) { return getMag() <= v2.getMag(); }
bool Vec4::operator>(Vec4 v2) { return getMag() > v2.getMag(); }
bool Vec4::operator<(Vec4 v2) { return getMag() < v2.getMag(); }
bool Vec4::operator==(float a) { return getMag() == a; }
bool Vec4::operator!=(float a) { return getMag() != a; }
bool Vec4::operator>=(float a) { return getMag() >= a; }
bool Vec4::operator<=(float a) { return getMag() <= a; }
bool Vec4::operator>(float a) { return getMag() > a; }
bool Vec4::operator<(float a) { return getMag() < a; }
bool Vec4::operator==(int a) { return getMag() == a; }
bool Vec4::operator!=(int a) { return getMag() != a; }
bool Vec4::operator>=(int a) { return getMag() >= a; }
bool Vec4::operator<=(int a) { return getMag() <= a; }
bool Vec4::operator>(int a) { return getMag() > a; }
bool Vec4::operator<(int a) { return getMag() < a; }
float &Vec4::operator[](int a) { return data[a]; }