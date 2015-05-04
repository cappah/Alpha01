#pragma once

#include "vec3.h"
#include "vec4.h"
#include "maths_func.h"

namespace gearengine { namespace maths {

	struct mat4
	{
		union
		{
			float m[4 * 4];
			vec4 columns[4];
		};

		mat4();
		mat4(float diagonal);

		vec4 getColumn(int index)
		{
			index *= 4;
			return vec4(m[index], m[index + 1], m[index + 2], m[index + 3]);
		}

		static mat4 identity();

		mat4& multiply(const mat4& other);
		friend mat4 operator*(mat4 left, const mat4& right);
		mat4& operator*=(const mat4& other);

		vec3 multiply(const vec3& other) const;
		friend vec3 operator*(const mat4& left, const vec3& right);

		vec4 multiply(const vec4& other) const;
		friend vec4 operator*(const mat4& left, const vec4& right);

		mat4& invert();

		static mat4 orthographic(float left, float right, float bottom, float top, float near, float far);
		static mat4 perspective(float fov, float aspectRatio, float near, float far);

		static mat4 translate(const vec3& translate);
		static mat4 rotate(float angle, const vec3& axis);
		static mat4 scale(const vec3& scale);
		static mat4 lookAt(const vec3& eye, const vec3& target, const vec3& up);

		// Barycentric coordinates on triangles
		static float baryCentric(vec3 p1, vec3 p2, vec3 p3, vec3 pos);
	};

} }