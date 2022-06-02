#ifndef VEC_H
#define VEC_H

#include <iostream>

// Ultra fast and basic vector class
// In R3 and based on double

struct vec {
	double x;
	double y;
	double z;

	// Constructors
    vec();
	vec(const double x, const double y, const double z);
	vec(const vec& other);
	vec operator=(const vec other);

	// Useful methods
	double norm() const;
	// Norm squared
	double norm2() const;
	vec normalized() const;
	vec project(const vec other) const;

	
	// Operators
	vec operator+(const vec other) const;
	vec operator-(const vec other) const;
	vec operator+=(const vec other);
	vec operator-=(const vec other);
	// Dot
	double operator*(const vec other) const;

	// Operators with numbers
	template <class T>
	vec operator*(const T number) const;
	template <class T>
	vec operator/(const T number) const;
	template <class T>
	vec operator*=(const T number);
	template <class T>
	vec operator/=(const T number);
};


// Other operations
vec cross(const vec v1, const vec v2);
double dot(const vec v1, const vec v2);
bool operator<(const vec& v1, const vec& v2);
bool operator==(const vec& v1, const vec& v2);
bool operator!=(const vec& v1, const vec& v2);
std::ostream& operator<<(std::ostream& os, const vec& v);

template <class T>
vec operator*(const T number, const vec v);



// Template definitions ----------------------------------------------------
template <class T>
vec vec::operator*(const T number) const
{
	vec ret = *this;
	ret.x *= number;
	ret.y *= number;
	ret.z *= number;
	return ret;
}

template <class T>
vec vec::operator/(const T number) const
{
	vec ret = *this;
	ret.x /= number;
	ret.y /= number;
	ret.z /= number;
	return ret;
}

template <class T>
vec vec::operator*=(const T number)
{
	*this = *this * number;
	return *this;
}

template <class T>
vec vec::operator/=(const T number)
{
	*this = *this / number;
	return *this;
}

template <class T>
vec operator*(const T number, const vec v)
{
	vec ret = v;
	ret.x *= number;
	ret.y *= number;
	ret.z *= number;
	return ret;
}

#endif
