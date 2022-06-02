#include "Vec.h"
#include <cmath>


// Constructors -----------------------------------------
vec::vec() :
    x(0.0), y(0.0), z(0.0)
{}

vec::vec(const double x, const double y, const double z) :
	x(x), y(y), z(z)
{}

vec::vec(const vec& other) :
	x(other.x), y(other.y), z(other.z)
{}

vec vec::operator=(const vec other)
{
	this->x = other.x;
	this->y = other.y;
	this->z = other.z;
	return *this;
}


// Methods -------------------------------------------------
double vec::norm() const
{
	return std::sqrt(this->norm2());
}

double vec::norm2() const
{
	return x*x + y*y + z*z;
}
vec vec::normalized() const
{
	return *this / this->norm();
}

vec vec::project(const vec other) const
{
	return dot(*this, other) * other / other.norm2();
}

// Operators -----------------------------------------------
vec vec::operator+(const vec other) const
{
	vec ret;
	ret.x = x + other.x;
	ret.y = y + other.y;
	ret.z = z + other.z;
	return ret;
}

vec vec::operator-(const vec other) const
{
	vec ret;
	ret.x = x - other.x;
	ret.y = y - other.y;
	ret.z = z - other.z;
	return ret;
}

vec vec::operator+=(const vec other)
{
	*this = *this + other;
	return *this;
}

vec vec::operator-=(const vec other)
{
	*this = *this - other;
	return *this;
}

double vec::operator*(const vec other) const
{
	return x * other.x + y * other.y + z * other.z;
}


// Operations -----------------------------------------------
vec cross(const vec v1, const vec v2)
{
	vec ret;
	ret.x = v1.y*v2.z - v1.z*v2.y;
	ret.y = v1.z*v2.x - v1.x*v2.z;
	ret.z = v1.x*v2.y - v1.y*v2.x;
	return ret;
}

double dot(const vec v1, const vec v2)
{
	return v1 * v2;
}

bool operator<(const vec& v1, const vec& v2)
{
    return v1.norm2() < v2.norm2();
}


bool operator==(const vec& v1, const vec& v2)
{
    return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

bool operator!=(const vec& v1, const vec& v2)
{
    return !(v1 == v2);
}

std::ostream& operator<<(std::ostream& os, const vec& v)
{
    os << v.x << '\t' << v.y << '\t' << v.z;
    return os;
}
