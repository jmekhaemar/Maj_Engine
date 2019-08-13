#include "resources/LoaderFont.h"

#include <Logging.h>

namespace maj::resources
{
	LoaderFont::LoaderFont()
		: Service { typeid(LoaderFont).name() }, mLibrary { nullptr }, mFace { nullptr }
	{
		FT_Error error { FT_Init_FreeType(&mLibrary) };
		if (error)
			THROW("failed to init freetype library with error: " + std::to_string(error))
	}

	LoaderFont::~LoaderFont()
	{
		if (mFace)
			FT_Done_Face(mFace);

		if (mLibrary)
			FT_Done_FreeType(mLibrary);
	}

	void LoaderFont::load(const char* kPath, gui::Font::FontInfo& info, std::unordered_map<uint32_t, gui::Glyph>& glyphs,
						  std::unordered_map<uint32_t, uint32_t>& charMap)
	{
		if (mFace)
			FT_Done_Face(mFace);

		FT_Error error { FT_New_Face(mLibrary, kPath, static_cast<uint32_t>(info.mFaceIndex), &mFace) };
		if (error)
			THROW("failed to open file with error: " + std::to_string(error))

		// Setting size in pixel, from point size (in 1/64th of a pixel), 300 dpi
		error = FT_Set_Char_Size(mFace, 0u, info.mBitmapSize << 6u, 300u, 300u);
		if (error)
			THROW("failed to set character size with error: " + std::to_string(error))

		info.mLineSize = static_cast<uint16_t>(mFace->size->metrics.ascender - mFace->size->metrics.descender) >> 6u;
		info.mMaxAdvance = static_cast<uint16_t>(mFace->size->metrics.max_advance) >> 6u;
		parseCharacters(glyphs, charMap);
	}

	void LoaderFont::parseCharacters(std::unordered_map<uint32_t, gui::Glyph>& glyphs, std::unordered_map<uint32_t, uint32_t>& charMap)
	{
		if (!mFace)
			return;

		// Only supports Unicode for now
		FT_Error error { FT_Select_Charmap(mFace, FT_ENCODING_UNICODE) };
		if (error)
			THROW("failed to select unicode character map with error: " + std::to_string(error))

		uint32_t glyphIndex { 0u };
		uint32_t charcode { FT_Get_First_Char(mFace, &glyphIndex) };

		while (glyphIndex != 0u)
		{
			if (glyphs.count(glyphIndex) == 0u)
				glyphs.emplace(glyphIndex, parseGlyph(glyphIndex));

			charMap.emplace(charcode, glyphIndex);
			charcode = FT_Get_Next_Char(mFace, charcode, &glyphIndex);
		}
	}

	gui::Glyph LoaderFont::parseGlyph(uint32_t glyphIndex)
	{
		FT_Error error { FT_Load_Glyph(mFace, glyphIndex, FT_LOAD_RENDER) };
		if (error)
			THROW("failed to load glyph " + std::to_string(glyphIndex) + " with error: " + std::to_string(error))

		// Bitmap top inversed for bearing (screen y axis pointing downwards), advance is converted to pixel
		gui::Glyph::Info info { {	mFace->glyph->bitmap.width, mFace->glyph->bitmap.rows }, { mFace->glyph->bitmap_left,  -mFace->glyph->bitmap_top },
									static_cast<uint32_t>(mFace->glyph->advance.x) >> 6u };

		return { std::move(info), mFace->glyph->bitmap.buffer };
	}
}