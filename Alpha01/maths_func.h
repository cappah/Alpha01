#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <glm\glm.hpp>

#define FOR(q,n) for(int q=0;q<n;q++)
#define SFOR(q,s,e) for(int q=s;q<=e;q++)
#define RFOR(q,n) for(int q=n;q>=0;q--)
#define RSFOR(q,s,e) for(int q=s;q>=e;q--)

#define ESZ(elem) (int)elem.size()

namespace gearengine 
{ 
	namespace maths 
	{

		// Degrees to radians
		inline float toRadians(float degrees)
		{
			return degrees * (M_PI / 180.0f);
		}

		// Radians to degrees
		inline float toDegrees(float radians)
		{
			return radians * (180.0f / M_PI);
		}


		template<typename T>
		inline T Clamp(const T &a, const T &min, const T &max)
		{
			if (a < min)
				return min;
			else if (a > max)
				return max;
			else
				return a;
		}


		inline float magnitude(const glm::vec3 v)
		{
			return sqrt(v.length());
		}

		static long floatToLong(float f)
		{
			// Converts a floating point number into an integer.
			// Fractional values are truncated as in ANSI C.
			// About 5 to 6 times faster than a standard typecast to an integer.

			long fpBits = *reinterpret_cast<const long*>(&f);
			long shift = 23 - (((fpBits & 0x7fffffff) >> 23) - 127);
			long result = ((fpBits & 0x7fffff) | (1 << 23)) >> shift;

			result = (result ^ (fpBits >> 31)) - (fpBits >> 31);
			result &= ~((((fpBits & 0x7fffffff) >> 23) - 127) >> 31);

			return result;
		}


		static float random(float min, float max)
		{
			// Returns a random number in range [min,max].

			return min + (max - min)
				* (static_cast<float>(rand()) / static_cast<float>(RAND_MAX));
		}


		template <typename T>
		static T bilerp(const T &a, const T &b, const T &c, const T &d, float u, float v)
		{
			// Performs a bilinear interpolation.
			//  P(u,v) = e + v(f - e)
			//  
			//  where
			//  e = a + u(b - a)
			//  f = c + u(d - c)
			//  u in range [0,1]
			//  v in range [0,1]

			return a * ((1.0f - u) * (1.0f - v))
				+ b * (u * (1.0f - v))
				+ c * (v * (1.0f - u))
				+ d * (u * v);
		}


		template <typename T>
		static T lerp(const T &a, const T &b, float t)
		{
			// Performs a linear interpolation.
			//  P(t) = (1 - t)a + tb
			//       = a + t(b - a)
			//
			//  where
			//  t in range [0,1]

			return a + (b - a) * t;
		}


	} 
}