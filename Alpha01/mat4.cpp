#include "mat4.h"

namespace gearengine { namespace maths {

	/*
	Initialize the m of a 4x4 matrix to 0.0f.
	---------
	|0|0|0|0|
	---------
	|0|0|0|0|
	---------
	|0|0|0|0|
	---------
	|0|0|0|0|
	---------
	*/
	mat4::mat4()
	{
		for (int i = 0; i < 4 * 4; i++)
			m[i] = 0.0f;
	}

	/*
	Initialize the diagonal m to 1.0f.
	---------
	|1|0|0|0|
	---------
	|0|1|0|0|
	---------
	|0|0|1|0|
	---------
	|0|0|0|1|
	---------
	*/
	mat4::mat4(float diagonal)
	{
		for (int i = 0; i < 4 * 4; i++)
			m[i] = 0.0f;

		m[0 + 0 * 4] = diagonal;
		m[1 + 1 * 4] = diagonal;
		m[2 + 2 * 4] = diagonal;
		m[3 + 3 * 4] = diagonal;
	}


	/* Initialize to default diagonal values */
	mat4 mat4::identity()
	{
		return mat4(1.0f);
	}

	mat4& mat4::multiply(const mat4& other)
	{
		float data[16];
		for (int y = 0; y < 4; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				float sum = 0.0f;
				for (int e = 0; e < 4; e++)
				{
					sum += m[x + e * 4] * other.m[e + y * 4];
				}
				data[x + y * 4] = sum;
			}
		}
		memcpy(m, data, 4 * 4 * sizeof(float));

		return *this;
	}

	vec3 mat4::multiply(const vec3& other) const
	{
		return vec3(
			columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x,
			columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y,
			columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z
			);
	}

	vec4 mat4::multiply(const vec4& other) const
	{
		return vec4(
			columns[0].x * other.x + columns[1].x * other.y + columns[2].x * other.z + columns[3].x * other.w,
			columns[0].y * other.x + columns[1].y * other.y + columns[2].y * other.z + columns[3].y * other.w,
			columns[0].z * other.x + columns[1].z * other.y + columns[2].z * other.z + columns[3].z * other.w,
			columns[0].w * other.x + columns[1].w * other.y + columns[2].w * other.z + columns[3].w * other.w
			);
	}

	mat4 operator*(mat4 left, const mat4& right)
	{
		return left.multiply(right);
	}

	mat4& mat4::operator*=(const mat4& other)
	{
		return multiply(other);
	}

	vec3 operator*(const mat4& left, const vec3& right)
	{
		return left.multiply(right);
	}

	vec4 operator*(const mat4& left, const vec4& right)
	{
		return left.multiply(right);
	}

	mat4& mat4::invert()
	{
		double temp[16];

		temp[0] = m[5] * m[10] * m[15] -
			m[5] * m[11] * m[14] -
			m[9] * m[6] * m[15] +
			m[9] * m[7] * m[14] +
			m[13] * m[6] * m[11] -
			m[13] * m[7] * m[10];

		temp[4] = -m[4] * m[10] * m[15] +
			m[4] * m[11] * m[14] +
			m[8] * m[6] * m[15] -
			m[8] * m[7] * m[14] -
			m[12] * m[6] * m[11] +
			m[12] * m[7] * m[10];

		temp[8] = m[4] * m[9] * m[15] -
			m[4] * m[11] * m[13] -
			m[8] * m[5] * m[15] +
			m[8] * m[7] * m[13] +
			m[12] * m[5] * m[11] -
			m[12] * m[7] * m[9];

		temp[12] = -m[4] * m[9] * m[14] +
			m[4] * m[10] * m[13] +
			m[8] * m[5] * m[14] -
			m[8] * m[6] * m[13] -
			m[12] * m[5] * m[10] +
			m[12] * m[6] * m[9];

		temp[1] = -m[1] * m[10] * m[15] +
			m[1] * m[11] * m[14] +
			m[9] * m[2] * m[15] -
			m[9] * m[3] * m[14] -
			m[13] * m[2] * m[11] +
			m[13] * m[3] * m[10];

		temp[5] = m[0] * m[10] * m[15] -
			m[0] * m[11] * m[14] -
			m[8] * m[2] * m[15] +
			m[8] * m[3] * m[14] +
			m[12] * m[2] * m[11] -
			m[12] * m[3] * m[10];

		temp[9] = -m[0] * m[9] * m[15] +
			m[0] * m[11] * m[13] +
			m[8] * m[1] * m[15] -
			m[8] * m[3] * m[13] -
			m[12] * m[1] * m[11] +
			m[12] * m[3] * m[9];

		temp[13] = m[0] * m[9] * m[14] -
			m[0] * m[10] * m[13] -
			m[8] * m[1] * m[14] +
			m[8] * m[2] * m[13] +
			m[12] * m[1] * m[10] -
			m[12] * m[2] * m[9];

		temp[2] = m[1] * m[6] * m[15] -
			m[1] * m[7] * m[14] -
			m[5] * m[2] * m[15] +
			m[5] * m[3] * m[14] +
			m[13] * m[2] * m[7] -
			m[13] * m[3] * m[6];

		temp[6] = -m[0] * m[6] * m[15] +
			m[0] * m[7] * m[14] +
			m[4] * m[2] * m[15] -
			m[4] * m[3] * m[14] -
			m[12] * m[2] * m[7] +
			m[12] * m[3] * m[6];

		temp[10] = m[0] * m[5] * m[15] -
			m[0] * m[7] * m[13] -
			m[4] * m[1] * m[15] +
			m[4] * m[3] * m[13] +
			m[12] * m[1] * m[7] -
			m[12] * m[3] * m[5];

		temp[14] = -m[0] * m[5] * m[14] +
			m[0] * m[6] * m[13] +
			m[4] * m[1] * m[14] -
			m[4] * m[2] * m[13] -
			m[12] * m[1] * m[6] +
			m[12] * m[2] * m[5];

		temp[3] = -m[1] * m[6] * m[11] +
			m[1] * m[7] * m[10] +
			m[5] * m[2] * m[11] -
			m[5] * m[3] * m[10] -
			m[9] * m[2] * m[7] +
			m[9] * m[3] * m[6];

		temp[7] = m[0] * m[6] * m[11] -
			m[0] * m[7] * m[10] -
			m[4] * m[2] * m[11] +
			m[4] * m[3] * m[10] +
			m[8] * m[2] * m[7] -
			m[8] * m[3] * m[6];

		temp[11] = -m[0] * m[5] * m[11] +
			m[0] * m[7] * m[9] +
			m[4] * m[1] * m[11] -
			m[4] * m[3] * m[9] -
			m[8] * m[1] * m[7] +
			m[8] * m[3] * m[5];

		temp[15] = m[0] * m[5] * m[10] -
			m[0] * m[6] * m[9] -
			m[4] * m[1] * m[10] +
			m[4] * m[2] * m[9] +
			m[8] * m[1] * m[6] -
			m[8] * m[2] * m[5];

		double determinant = m[0] * temp[0] + m[1] * temp[4] + m[2] * temp[8] + m[3] * temp[12];
		determinant = 1.0 / determinant;

		for (int i = 0; i < 4 * 4; i++)
			m[i] = temp[i] * determinant;

		return *this;
	}


	/*
	Initialize the orthographic projection.
	---------
	|2/(r-l)|0|0|0|
	---------
	|0|2/(t-b)|0|0|
	---------
	|0|0|2/(n-f)|0|
	---------
	|(l+r)/(l-r)|(b+t)/(b-t)|(f+n)/(n+f)|1|
	---------
	*/
	mat4 mat4::orthographic(float left, float right, float bottom, float top, float near, float far)
	{
		mat4 result(1.0f);

		result.m[0 + 0 * 4] = 2.0f / (right - left);

		result.m[1 + 1 * 4] = 2.0f / (top - bottom);

		result.m[2 + 2 * 4] = 2.0f / (near - far);

		result.m[0 + 3 * 4] = (left + right) / (left - right);
		result.m[1 + 3 * 4] = (bottom + top) / (bottom - top);
		result.m[2 + 3 * 4] = (far + near) / (far - near);

		return result;
	}


	mat4 mat4::perspective(float fov, float aspectRatio, float near, float far)
	{
		mat4 result(1.0f);

		float q = 1.0f / tan(toRadians(0.5f * fov));
		float a = q / aspectRatio;

		float b = (near + far) / (near - far);
		float c = (2.0f * near * far) / (near - far);

		result.m[0 + 0 * 4] = a;
		result.m[1 + 1 * 4] = q;
		result.m[2 + 2 * 4] = b;
		result.m[3 + 2 * 4] = -1.0f;
		result.m[2 + 3 * 4] = c;

		return result;
	}

	mat4 mat4::translate(const vec3& translate)
	{
		mat4 result(1.0f);

		result.m[0 + 3 * 4] = translate.x;
		result.m[1 + 3 * 4] = translate.y;
		result.m[2 + 3 * 4] = translate.z;

		return result;
	}

	mat4 mat4::rotate(float angle, const vec3& axis)
	{
		mat4 result(1.0f);

		float r = toRadians(angle);
		float c = cos(r);
		float s = sin(r);
		float omc = 1.0f - c;

		float x = axis.x;
		float y = axis.y;
		float z = axis.z;

		result.m[0 + 0 * 4] = x * omc + c;
		result.m[1 + 0 * 4] = y * x * omc + z * s;
		result.m[2 + 0 * 4] = x * z * omc - y * s;

		result.m[0 + 1 * 4] = x * y * omc - z * s;
		result.m[1 + 1 * 4] = y * omc + c;
		result.m[2 + 1 * 4] = y * z * omc + x * s;

		result.m[0 + 2 * 4] = x * z * omc + y * s;
		result.m[1 + 2 * 4] = y * z * omc - x * s;
		result.m[2 + 2 * 4] = z * omc + c;

		return result;
	}

	mat4 mat4::scale(const vec3& scale)
	{
		mat4 result(1.0f);

		result.m[0 + 0 * 4] = scale.x;
		result.m[1 + 1 * 4] = scale.y;
		result.m[2 + 2 * 4] = scale.z;

		return result;
	}


	/*
	---------
	|R|R|R|d|
	---------
	|R|R|R|d|
	---------
	|R|R|R|d|
	---------
	|0|0|0|0|
	---------
	*/
	mat4 mat4::lookAt(const vec3& eye, const vec3& target, const vec3& up)
	{
		mat4 result(1.0f);

		vec3 f(vec3::normalize(target - eye));
		vec3 s(vec3::normalize(vec3::cross(f, up)));
		vec3 u(vec3::normalize(vec3::cross(s, f)));

		// FIRST ROW 
		result.m[0 + 0 * 4] = s.x;
		result.m[1 + 0 * 4] = s.y;
		result.m[2 + 0 * 4] = s.z;

		// SECOND ROW
		result.m[0 + 1 * 4] = u.x;
		result.m[1 + 1 * 4] = u.y;
		result.m[2 + 1 * 4] = u.z;

		// THIRD ROW
		result.m[0 + 2 * 4] = f.x;
		result.m[1 + 2 * 4] = f.y;
		result.m[2 + 2 * 4] = f.z;

		// FOURTH COLUmN
		result.m[3 + 0 * 4] = -vec3::dot(s, eye);
		result.m[3 + 1 * 4] = -vec3::dot(u, eye);
		result.m[3 + 2 * 4] = vec3::dot(f, eye);

		return result;
	}


	float mat4::baryCentric(vec3 p1, vec3 p2, vec3 p3, vec3 pos)
	{
		float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
		float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
		float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
		float l3 = 1.0f - l1 - l2;

		return l1 * p1.y + l2 * p2.y + l3 * p3.y;
	}

} }