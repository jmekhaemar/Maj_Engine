#include "Color.h"

namespace maj
{
	DEFINE_HASH_OVERLOAD(Color)

	Color::Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A)
		:math::Vec4f{(static_cast<float>(R) / LIMIT_UINT),(static_cast<float>(G) / LIMIT_UINT),(static_cast<float>(B) / LIMIT_UINT),(static_cast<float>(A) / LIMIT_UINT) }
	{
	}
	
	void Color::initialize()
	{
	}
	
	void Color::r(uint8_t val)
	{
		mX = static_cast<float>(val) / LIMIT_UINT;
	}

	void Color::g(uint8_t val)
	{
		mY = static_cast<float>(val) / LIMIT_UINT;
	}

	void Color::b(uint8_t val)
	{
		mZ = static_cast<float>(val) / LIMIT_UINT;
	}
	void Color::a(uint8_t val)
	{
		mW = static_cast<float>(val) / LIMIT_UINT;
	}

	uint8_t Color::r()const
	{
		return static_cast<uint8_t>(mX * LIMIT_UINT);
	}

	uint8_t Color::g()const
	{
		return static_cast<uint8_t>(mY * LIMIT_UINT);
	}

	uint8_t Color::b()const
	{
		return static_cast<uint8_t>(mZ * LIMIT_UINT);
	}

	uint8_t Color::a()const
	{
		return static_cast<uint8_t>(mW * LIMIT_UINT);
	}

	math::Vec3<uint8_t> Color::rgb()const
	{
		return { static_cast<uint8_t>(mX * LIMIT_UINT) , static_cast<uint8_t>(mY * LIMIT_UINT) , static_cast<uint8_t>(mZ * LIMIT_UINT) };
	}

	math::Vec4<uint8_t> Color::rgba()const
	{
		return { static_cast<uint8_t>(mX * LIMIT_UINT) , static_cast<uint8_t>(mY * LIMIT_UINT) , static_cast<uint8_t>(mZ * LIMIT_UINT), static_cast<uint8_t>(mW * LIMIT_UINT) };
	}

	void Color::rgb(uint8_t valR, uint8_t valG, uint8_t valB) 
	{
		mX = static_cast<float>(valR) / LIMIT_UINT;
		mY = static_cast<float>(valG) / LIMIT_UINT;
		mZ = static_cast<float>(valB) / LIMIT_UINT;
	}

	void Color::rgba(uint8_t valR, uint8_t valG, uint8_t valB, uint8_t valA)
	{
		mX = static_cast<float>(valR) / LIMIT_UINT;
		mY = static_cast<float>(valG) / LIMIT_UINT;
		mZ = static_cast<float>(valB) / LIMIT_UINT;
		mW = static_cast<float>(valA) / LIMIT_UINT;
	}
};