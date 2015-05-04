#pragma once
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

namespace gearengine 
{
	namespace maths 
	{
		class Quaternion
		{
		public:
			Quaternion();
			Quaternion(float x, float y, float z, float w);
			Quaternion(const vec4& r);
			Quaternion(const vec3& axis, float angle);
			Quaternion(const mat4& matrix);
			~Quaternion();


		private:
			vec4 quat;
		};

	}
}