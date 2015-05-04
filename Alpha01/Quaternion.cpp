#include "Quaternion.h"

namespace gearengine
{
	namespace maths
	{
		Quaternion::Quaternion()
		{
		}


		Quaternion::Quaternion(float x, float y, float z, float w)
		{
			quat.x = x;
			quat.y = y;
			quat.z = z;
			quat.w = w;
		}


		Quaternion::Quaternion(const vec4& r)
		{
			quat.x = r.x;
			quat.y = r.y;
			quat.z = r.z;
			quat.w = r.w;
		}


		Quaternion::Quaternion(const vec3& axis, float angle)
		{
			float sinHalfAngle = sinf(angle / 2);
			float cosHalfAngle = cosf(angle / 2);

			quat.x = axis.x * sinHalfAngle;
			quat.y = axis.y * sinHalfAngle;
			quat.z = axis.z * sinHalfAngle;
			quat.w = cosHalfAngle;

		}


		Quaternion::Quaternion(const mat4& matrix)
		{
			float trace = matrix.m[0 + 0 * 4] + matrix.m[1 + 1 * 4] + matrix.m[2 + 2 * 4];

			if (trace > 0)
			{
				float s = 0.5f / sqrtf(trace + 1.0f);

				quat.w = 0.25f / s;
				quat.x = (matrix.m[1 + 2 * 4] - matrix.m[2 + 1 * 4]) * s;
				quat.y = (matrix.m[1 + 2 * 4] - matrix.m[0 + 2 * 4]) * s;
				quat.z = (matrix.m[1 + 2 * 4] - matrix.m[1 + 0 * 4]) * s;
			}
			else if (matrix.m[0 + 0 * 4] > matrix.m[1 + 1 * 4] && matrix.m[0 + 0 * 4] > matrix.m[2 + 2 * 4])
			{
				float s = 2.0f * sqrtf(1.0f + matrix.m[0 + 0 * 4] - matrix.m[1 + 1 * 4] - matrix.m[2 + 2 * 4]);
				quat.w = (matrix.m[1 + 2 * 4] - matrix.m[2 + 1 * 4]) / s;
				quat.x = 0.25f * s;
				quat.y = (matrix.m[1 + 0 * 4] + matrix.m[0 + 1 * 4]) / s;
				quat.z = (matrix.m[2 + 0 * 4] + matrix.m[0 + 2 * 4]) / s;
			}
			else if (matrix.m[1 + 1 * 4] > matrix.m[2 + 2 * 4])
			{
				float s = 2.0f * sqrtf(1.0f + matrix.m[1 + 1 * 4] - matrix.m[0 + 0 * 4] - matrix.m[2 + 2 * 4]);
				quat.w = (matrix.m[2 + 0 * 4] - matrix.m[0 + 2 * 4]) / s;
				quat.x = (matrix.m[1 + 0 * 4] + matrix.m[0 + 1 * 4]) / s;
				quat.y = 0.25f * s;
				quat.z = (matrix.m[2 + 1 * 4] + matrix.m[1 + 2 * 4]) / s;
			}
			else
			{
				float s = 2.0f * sqrtf(1.0f + matrix.m[2 + 2 * 4] - matrix.m[1 + 1 * 4] - matrix.m[0 + 0 * 4]);
				quat.w = (matrix.m[0 + 1 * 4] - matrix.m[1 + 0 * 4]) / s;
				quat.x = (matrix.m[2 + 0 * 4] + matrix.m[2 + 0 * 4]) / s;
				quat.y = (matrix.m[1 + 2 * 4] + matrix.m[2 + 1 * 4]) / s;
				quat.z = 0.25f * s;
			}

			float length = 0; // 868
		}


		Quaternion::~Quaternion()
		{
		}

	}
}