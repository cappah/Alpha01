#include "vec3.h"

namespace gearengine { namespace maths {

	vec3::vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	vec3::vec3(const float& x, const float& y, const float& z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	vec3& vec3::add(const vec3& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;

		return *this;
	}

	vec3& vec3::subtract(const vec3& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;

		return *this;
	}

	vec3& vec3::multiply(const vec3& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;

		return *this;
	}

	vec3& vec3::divide(const vec3& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;

		return *this;
	}


	float vec3::dot(const vec3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}


	float vec3::dot(const vec3 v1, const vec3 v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}


	vec3 vec3::cross(const vec3 v)
	{
		float x_ = y * v.z - z * v.y;
		float y_ = z * v.x - x * v.z;
		float z_ = x * v.y - y * v.x;

		return vec3(x_, y_, z_);
	}


	vec3 vec3::cross(const vec3 v1, const vec3 v2)
	{
		float x_ = v1.y * v2.z - v1.z * v2.y;
		float y_ = v1.z * v2.x - v1.x * v2.z;
		float z_ = v1.x * v2.y - v1.y * v2.x;

		return vec3(x_, y_, z_);
	}


	vec3 vec3::normalize()
	{
		float x_ = x / length();
		float y_ = y / length();
		float z_ = z / length();

		return vec3(x_, y_, z_);
	}


	vec3 vec3::normalize(const vec3 v)
	{
		float x_ = v.x / length(v);
		float y_ = v.y / length(v);
		float z_ = v.z / length(v);

		return vec3(x_, y_, z_);
	}


	float vec3::length()
	{
		return sqrt(x * x + y * y + z * z);
	}


	float vec3::length(const vec3 v)
	{
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}


	void vec3::inverse()
	{
		x = -x;
		y = -y;
		z = -z;
	}


	float vec3::distance(const vec3 v1, const vec3 v2)
	{
		float _x = v1.x - v2.x;
		float _y = v1.y - v2.y;
		float _z = v1.z - v2.z;

		return sqrt(_x * _x + _y * _y + _z * _z);
	}


	vec3 operator+(vec3 left, const vec3& right)
	{
		return left.add(right);
	}

	vec3 operator-(vec3 left, const vec3& right)
	{
		return left.subtract(right);
	}

	vec3 operator*(vec3 left, const vec3& right)
	{
		return left.multiply(right);
	}

	vec3 operator/(vec3 left, const vec3& right)
	{
		return left.divide(right);
	}

	vec3& vec3::operator+=(const vec3& other)
	{
		return add(other);
	}

	vec3& vec3::operator-=(const vec3& other)
	{
		return subtract(other);
	}

	vec3& vec3::operator*=(const vec3& other)
	{
		return multiply(other);
	}

	vec3& vec3::operator/=(const vec3& other)
	{
		return divide(other);
	}

	bool vec3::operator==(const vec3& other)
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool vec3::operator!=(const vec3& other)
	{
		return !(*this == other);
	}

	std::ostream& operator<<(std::ostream& stream, const vec3& vector)
	{
		stream << "vec3: (" << vector.x << ", " << vector.y << ", " << vector.z << ")";
		return stream;
	}


} }