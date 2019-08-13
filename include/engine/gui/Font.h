#ifndef _FONT_H_
#define _FONT_H_

#include <unordered_map>

#include <NonCopyable.h>

#include "gui/Glyph.h"

namespace maj::gui
{
	class Font final : public NonCopyable, public ISerializable
	{
		ENABLE_META(Font)

	public:
		struct FontInfo final : public ISerializable
		{
			ENABLE_META(FontInfo)

		public:
			LOAD_CONSTRUCTOR(FontInfo)

			FontInfo(uint8_t faceIndex = 0u, uint8_t size = 24u);
			~FontInfo() = default;

			uint8_t mFaceIndex;
			uint8_t mBitmapSize;
			uint16_t mLineSize;
			uint16_t mMaxAdvance;
		};

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Font)

		Font();
		Font(const char* kPath);
		Font(const char* kPath, FontInfo&& info);
		~Font();

		const FontInfo& info() const;
		const Glyph* glyph(uint32_t charCode) const;

		Event<void> mOnChange;

	private:
		const std::string mkPath;
		FontInfo mInfo;

		std::unordered_map<uint32_t, Glyph> mGlyphs;
		std::unordered_map<uint32_t, uint32_t> mCharMap;
	};
}

META_CLASS(maj::Asset<maj::gui::Font>, 1u)
META_FIELD(maj::Asset<maj::gui::Font>, uint32_t, mGuid, 0u)

META_CLASS(maj::gui::Font::FontInfo, 2u)
META_FIELD(maj::gui::Font::FontInfo, uint8_t, mFaceIndex, 0u)
META_FIELD(maj::gui::Font::FontInfo, uint8_t, mBitmapSize, 1u)

META_CLASS(maj::gui::Font, 3u)
META_FIELD(maj::gui::Font, uint32_t, mGuid, 0u)
META_FIELD(maj::gui::Font, const std::string, mkPath, 1u)
META_FIELD(maj::gui::Font, maj::gui::Font::FontInfo, mInfo, 2u)

#endif