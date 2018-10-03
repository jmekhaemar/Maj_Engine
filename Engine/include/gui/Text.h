#ifndef _GUI_TEXT_H_
#define _GUI_TEXT_H_

#include "gui/IElement.h"
#include "gui/Font.h"
#include "gui/Char.h"

namespace maj::gui
{
	class Text final : public IElement
	{
		ENABLE_META_CUSTOM(Text)

	private:
		struct Pen final
		{
			Pen(const Text& kText);
			~Pen() = default;

			void testExtent(const math::Vec2f& kCharExtent);
			void newLine();

			math::Vec2f mPosition;
			math::Vec2f mMaxExtent;
			float mOriginX;
			float mLineSize;
			bool mCanDraw;
		};

		static void format(std::string& str);
		static void unformat(std::string& str);

	public:
		LOAD_CONSTRUCTOR(Text)

		Text(CWidget* widget, const rhi::Material* kMaterial, const Font* kFont);
		virtual ~Text() = default;

		virtual void draw() const override;
		virtual void camera(const rhi::IDescriptor* kBuffer) override;

		const Font* font() const;
		void font(const Font* kFont);
		
		const std::string& text() const;
		void text(const std::string& kStr);
		void text(std::string&& str);

		uint32_t fontSize() const;
		void fontSize(uint32_t size);

		bool fitToSize() const;
		void fitToSize(bool fit);

		virtual void resize(const math::Vec2f& kFactors) override;

	private:
		virtual void onAttach() override;
		virtual void onSpaceChanged(ECoordSpace space) override;
		virtual void onSizeChanged() override;

		void makeCharacters();
		void drawCharacter(Pen& pen, uint32_t charIndex) const;
		void fitText();

		Asset<rhi::Material> mMaterial;
		Asset<Font> mFont;
		std::string mText;

		float mFontSize;
		bool mFitToSize;

		std::vector<std::unique_ptr<Char>> mChars;
	};
}

META_CLASS_PARENT(maj::gui::Text, maj::gui::IElement, 5u)
META_FIELD(maj::gui::Text, maj::Asset<maj::rhi::Material>, mMaterial, 0u)
META_FIELD(maj::gui::Text, maj::Asset<maj::gui::Font>, mFont, 1u)
META_FIELD(maj::gui::Text, std::string, mText, 2u) // TODO until serialization supports std::wstring
META_FIELD(maj::gui::Text, float, mFontSize, 3u)
META_FIELD(maj::gui::Text, bool, mFitToSize, 4u)

#endif