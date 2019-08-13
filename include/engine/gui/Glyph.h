#ifndef _GLYPH_H_
#define _GLYPH_H_

#include <memory>
#include <array>

#include <Vec2.h>
#include <Texture.h>

namespace maj::gui
{
	class Glyph final : public NonCopyable
	{
	public:
		struct Info
		{
			math::Vec2ui mSize;
			math::Vec2i mBearing;
			uint32_t mAdvance;
		};

		Glyph(Info&& info, const uint8_t* bitmap);
		Glyph(Glyph&& other);
		~Glyph();

		const Info& info() const;
		const rhi::Texture* bitmap() const;

	private:
		Info mInfo;
		std::unique_ptr<rhi::Image> mImage;
		std::unique_ptr<rhi::Texture> mTexture;
	};
}

#endif