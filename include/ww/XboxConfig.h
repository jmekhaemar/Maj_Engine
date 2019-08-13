#ifndef _XBOX_CONFIG_
#define _XBOX_CONFIG_

namespace maj::ww
{
	struct Xbox360 final
	{
		static constexpr char* skName { "Xbox 360 Controller" };

		enum class EKey : uint8_t
		{
			A = 0u,
			B = 1u,
			X = 2u,
			Y = 3u,
			LB = 4u,
			RB = 5u,
			BACK = 6u,
			START = 7u,
			LEFT_STICK = 8u,
			RIGHT_STICK = 9u,
			UP = 10u,
			RIGHT = 11u,
			DOWN = 12u,
			LEFT = 13u
		};

		enum class EAxis : uint8_t
		{
			LEFT_X = 0u,
			LEFT_Y = 1u,
			RIGHT_X = 2u,
			RIGHT_Y = 3u,
			LT = 4u,		// [-1, 1]
			RT = 5u,		// [-1, 1]
		};
	};
}

#endif