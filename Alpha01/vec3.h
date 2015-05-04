#pragma once

#include <iostream>

namespace gearengine { namespace maths {

	struct vec3
	{
		float x, y, z;

		vec3();
		vec3(const float& x, const float& y, const float& z);

		vec3& add(const vec3& other);
		vec3& subtract(const vec3& other);
		vec3& multiply(const vec3& other);
		vec3& divide(const vec3& other);

		float dot(const vec3 v);
		static float dot(const vec3 v1, const vec3 v2);

		vec3 cross(const vec3 v);
		static vec3 cross(const vec3 v1, const vec3 v2);

		vec3 normalize();
		static vec3 normalize(const vec3 v);

		float length();
		static float length(const vec3 v);

		void inverse();
		float distance(const vec3 v1, const vec3 v2);

		friend vec3 operator+(vec3 left, const vec3& right);
		friend vec3 operator-(vec3 left, const vec3& right);
		friend vec3 operator*(vec3 left, const vec3& right);
		friend vec3 operator/(vec3 left, const vec3& right);

		bool operator==(const vec3& other);
		bool operator!=(const vec3& other);

		vec3& operator+=(const vec3& other);
		vec3& operator-=(const vec3& other);
		vec3& operator*=(const vec3& other);
		vec3& operator/=(const vec3& other);

		friend std::ostream& operator<<(std::ostream& stream, const vec3& vector);
	};

} }