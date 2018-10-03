#ifndef _GUI_CHAR_H_
#define _GUI_CHAR_H_

#include <MaterialInstance.h>
#include <Buffer.h>

#include "gui/Glyph.h"

namespace maj::gui
{
	class Char final : public NonCopyable
	{
	public:
		Char(const rhi::Material* kMaterial, const Glyph* kGlyph, const math::Vec2f& kScreenFactor, bool displayed = true);
		Char(Char&& other);
		~Char();

		void material(const rhi::Material* kMaterial);

		const rhi::MaterialInstance* materialInstance() const;
		rhi::MaterialInstance* materialInstance();

		const Glyph* glyph() const;
		void glyph(const Glyph* kGlyph, const math::Vec2f& kScreenFactor);

		const math::Vec2f& bearing() const;
		const math::Vec2f& size() const;
		float advance() const;

		bool isDisplayed() const;
		void display(bool displayed);

		void draw(const math::Vec2f& kPen, float fontSize, const math::Vec4Holder<float>& kColor) const;
		void resize(const math::Vec2f& kScale);

	private:
		void retrieveGlyphInfo(const math::Vec2f& kScreenFactor);

		rhi::MaterialInstance* mMaterialInst;
		rhi::Buffer::SubBuffer* mBuffer;
		bool mIsDisplayed;

		const Glyph* mkGlyph;

		math::Vec2f mSize;
		math::Vec2f mBearing;
		float mAdvance;
	};
}

#endif