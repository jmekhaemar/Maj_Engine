#ifndef _COLOR_H_
#define _COLOR_H_

#include <Vec4.h>
#include <Vec3.h>
#include <limits>

#define LIMIT_UINT std::numeric_limits<uint8_t>::max()

namespace maj
{
	class Color : public math::Vec4f
	{

		ENABLE_META_PARENT(Color, math::Vec4f)

	public:
		LOAD_CONSTRUCTOR(Color)

		Color(uint8_t R = 255u, uint8_t G = 255u, uint8_t B = 255u, uint8_t A = 255u);
		~Color() = default;
		void r(uint8_t val);
		void g(uint8_t val);
		void b(uint8_t val);
		void a(uint8_t val);
		uint8_t r()const;
		uint8_t g()const;
		uint8_t b()const;
		uint8_t a()const;

		math::Vec3<uint8_t> rgb()const;
		math::Vec4<uint8_t> rgba()const;
		void rgb(uint8_t valR, uint8_t valG, uint8_t valB);
		void rgba(uint8_t valR, uint8_t valG, uint8_t valB, uint8_t valA);
	};
}

META_CLASS_PARENT(maj::Color, maj::math::Vec4f, 0u)

#endif