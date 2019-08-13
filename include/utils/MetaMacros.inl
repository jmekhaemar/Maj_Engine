#include "MetaMacros.h"

namespace maj::meta
{
	template<typename T>
	const char* Type<T>::sName { typeid(T).name() };

	template<typename T>
	size_t Type<T>::sSize { sizeof(T) };

	template<uint32_t start, uint32_t end, typename Func, uint32_t i, typename... Args>
	constexpr void staticFor(Args&&... args)
	{
		Func::template invoke<i>(std::forward<Args>(args)...);
		if constexpr (i < end)
			staticFor<start, end, Func, i + 1u>(std::forward<Args>(args)...);
	}

	constexpr uint32_t staticHash(const char* str)
	{
		return *str ? static_cast<uint32_t>(*str) + static_cast<uint32_t>(33u * static_cast<uint64_t>(staticHash(str + 1u))) : 5381u;
	}

	template<typename T>
	constexpr uint32_t Enum<T>::size() 
	{
		return 0u;
	}
}