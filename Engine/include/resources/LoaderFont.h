#ifndef _LOADER_FONT_H_
#define _LOADER_FONT_H_

#include <ft2build.h>
#include FT_FREETYPE_H

#include <Service.h>

#include "gui/Font.h"

namespace maj::resources
{
	class LoaderFont final : public Service
	{
	public:
		LoaderFont();
		~LoaderFont();

		void load(const char* kPath, gui::Font::FontInfo& info, std::unordered_map<uint32_t, gui::Glyph>& glyphs, std::unordered_map<uint32_t, uint32_t>& charMap);

	private:
		void parseCharacters(std::unordered_map<uint32_t, gui::Glyph>& glyphs, std::unordered_map<uint32_t, uint32_t>& charMap);
		gui::Glyph parseGlyph(uint32_t glyphIndex);

		FT_Library mLibrary;
		FT_Face mFace;
	};
}

#endif