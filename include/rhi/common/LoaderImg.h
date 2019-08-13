#ifndef _LOADER_IMG_H_
#define _LOADER_IMG_H_

#include <vector>

#include <Service.h>
#include <Vec2.h>

#include "common/Format.h"

namespace maj::rhi
{
	class LoaderImg final : public Service
	{
	public:
		LoaderImg();
		~LoaderImg() = default;

		void load(const char* kPath, std::vector<uint8_t>& data, math::Vec2ui& extent, const rhi::Format& kImgFormat);
	};
}

#endif