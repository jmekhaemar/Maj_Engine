#ifndef _MATH_HELPER_H_
#define _MATH_HELPER_H_

namespace maj::math
{
	#define PI 3.1415926535897932384626433f
	#define RAD2DEG 57.295779513082320876798154814105f
	#define DEG2RAD 0.0174532925199432957692369076849f

	template<typename T>
	class Helper final
	{	
	public:
		static const T skEpsilon;
		
		static T lerp(T start, T end, T alpha);

		static bool areSame(T a, T b);
	};
}

#include "Helper.inl"

#endif