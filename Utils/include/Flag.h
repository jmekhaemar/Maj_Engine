#ifndef _FLAG_H_
#define _FLAG_H_

#define DECLARE_FLAG(FLAG_NAME)																	\
		constexpr FLAG_NAME operator&(const FLAG_NAME& kSrc, const FLAG_NAME& kDst) noexcept;	\
		constexpr FLAG_NAME operator|(const FLAG_NAME& kSrc, const FLAG_NAME& kDst) noexcept;	\
		constexpr FLAG_NAME operator^(const FLAG_NAME& kSrc, const FLAG_NAME& kDst) noexcept;	\
		constexpr FLAG_NAME& operator&=(FLAG_NAME& src, const FLAG_NAME& kDst) noexcept;		\
		constexpr FLAG_NAME& operator|=(FLAG_NAME& src, const FLAG_NAME& kDst) noexcept;		\
		constexpr FLAG_NAME& operator^=(FLAG_NAME& src, const FLAG_NAME& kDst) noexcept;		\
		constexpr FLAG_NAME operator~(const FLAG_NAME& kSrc) noexcept;

#define DEFINE_FLAG(FLAG_NAME, FLAG_TYPE)																				\
		constexpr FLAG_NAME operator&(const FLAG_NAME& kSrc, const FLAG_NAME& kDst) noexcept							\
		{																												\
			return static_cast<FLAG_NAME>(static_cast<FLAG_TYPE>(kSrc) & static_cast<FLAG_TYPE>(kDst));					\
		}																												\
		constexpr FLAG_NAME operator|(const FLAG_NAME& kSrc, const FLAG_NAME& kDst) noexcept							\
		{																												\
			return static_cast<FLAG_NAME>(static_cast<FLAG_TYPE>(kSrc) | static_cast<FLAG_TYPE>(kDst));					\
		}																												\
		constexpr FLAG_NAME operator^(const FLAG_NAME& kSrc, const FLAG_NAME& kDst) noexcept							\
		{																												\
			return static_cast<FLAG_NAME>(static_cast<FLAG_TYPE>(kSrc) ^ static_cast<FLAG_TYPE>(kDst));					\
		}																												\
		constexpr FLAG_NAME& operator&=(FLAG_NAME& src, const FLAG_NAME& kDst) noexcept									\
		{																												\
			FLAG_TYPE temp { static_cast<FLAG_TYPE>(src) };																\
			temp &= static_cast<FLAG_TYPE>(kDst);																		\
			return src = static_cast<FLAG_NAME>(temp);																	\
		}																												\
		constexpr FLAG_NAME& operator|=(FLAG_NAME& src, const FLAG_NAME& kDst) noexcept									\
		{																												\
			FLAG_TYPE temp { static_cast<FLAG_TYPE>(src) };																\
			temp |= static_cast<FLAG_TYPE>(kDst);																		\
			return src = static_cast<FLAG_NAME>(temp);																	\
		}																												\
		constexpr FLAG_NAME& operator^=(FLAG_NAME& src, const FLAG_NAME& kDst) noexcept									\
		{																												\
			FLAG_TYPE temp { static_cast<FLAG_TYPE>(src) };																\
			temp ^= static_cast<FLAG_TYPE>(kDst);																		\
			return src = static_cast<FLAG_NAME>(temp);																	\
		}																												\
		constexpr FLAG_NAME operator~(const FLAG_NAME& kSrc) noexcept													\
		{																												\
			return static_cast<FLAG_NAME>(~static_cast<FLAG_TYPE>(kSrc));												\
		}

#endif