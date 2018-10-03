#include "gui/Font.h"

#include "resources/LoaderFont.h"

namespace maj::gui
{
	DEFINE_HASH_OVERLOAD(Font::FontInfo)
	DEFINE_HASH_OVERLOAD(Font)

	Font::FontInfo::FontInfo(uint8_t faceIndex, uint8_t size)
		: mFaceIndex { faceIndex }, mBitmapSize { size }, mLineSize { 0u }, mMaxAdvance { 0u }
	{}

	void Font::FontInfo::initialize()
	{}

	Font::Font(JsonNode* root)
		: mkPath { "" }, mInfo {}, mGlyphs {}, mCharMap {}
	{
		load(root);
		initialize();
	}

	Font::Font() 
		: mkPath{ }, mInfo{}, mGlyphs{}, mCharMap{}
	{
		initialize();
	}

	Font::Font(const char* kPath)
		: mkPath { kPath }, mInfo {}, mGlyphs {}, mCharMap {}
	{
		initialize();
	}

	Font::Font(const char* kPath, FontInfo&& info)
		: mkPath { kPath }, mInfo { std::move(info) }, mGlyphs {}, mCharMap {}
	{
		initialize();
	}

	Font::~Font()
	{}

	void Font::initialize()
	{
		if (mOnChange.empty())
		{
			mOnChange += [this]() -> void
			{
				mGlyphs.clear();
				mCharMap.clear();

				initialize();
			};
		}

		try
		{
			Service::get<resources::LoaderFont>()->load(mkPath.c_str(), mInfo, mGlyphs, mCharMap);
		}
		catch (const std::exception& e)
		{
#ifdef MAJ_EDITOR
			std::string result{ "failed to load font \'" + mkPath + "\': " + std::string{ e.what() } };
			LOG(LogWarning, result);
#else
			THROW("failed to load font \'" + mkPath + "\': " + std::string{ e.what() })
#endif
		}
	}

	const Font::FontInfo& Font::info() const
	{
		return mInfo;
	}

	const Glyph* Font::glyph(uint32_t charCode) const
	{
		uint32_t glyphIndex { mCharMap.count(charCode) == 0u ? 0u : mCharMap.at(charCode) };
		if (mGlyphs.count(glyphIndex) == 0u)
			return nullptr;

		return &mGlyphs.at(glyphIndex);
	}
}