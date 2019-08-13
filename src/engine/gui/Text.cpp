#include "gui/Text.h"

#include "scene/SceneHandler.h"
#include "gui/CWidget.h"

namespace maj::gui
{
	DEFINE_HASH_OVERLOAD(Text)

	Text::Pen::Pen(const Text& kText)
		: mPosition { kText.mPosition }, mMaxExtent { mPosition + kText.mSize }, mOriginX { mPosition.mX }, mLineSize {}, mCanDraw { true }
	{
		mLineSize = (static_cast<float>(kText.mFont->info().mLineSize) / static_cast<float>(kText.mWidget->reference().mY)) * kText.mFontSize;
		mPosition.mY += mLineSize;
	}

	void Text::Pen::testExtent(const math::Vec2f& kCharExtent)
	{
		if (kCharExtent.mX <= mMaxExtent.mX)
			return;

		newLine();
	}

	void Text::Pen::newLine()
	{
		mPosition.mY += mLineSize;
		mPosition.mX = mOriginX;

		if (mPosition.mX > mMaxExtent.mX || mPosition.mY > mMaxExtent.mY)
			mCanDraw = false;
	}

	void Text::format(std::string& str)
	{
		for (char& c : str)
		{
			if (c == '\n')
				c = '#';
		}
	}

	void Text::unformat(std::string& str)
	{
		for (char& c : str)
		{
			if (c == '#')
				c = '\n';
		}
	}

	Text::Text(CWidget* widget, const rhi::Material* kMaterial, const Font* kFont)
		: IElement { widget }, mMaterial { kMaterial }, mFont { kFont }, mText {}, mFontSize { 1.f }, mFitToSize { false }, mChars {}
	{
		initialize();
		onAttach();
	}
	
	SAVE_FILE_CUSTOM_SIGNATURE(Text)
	{
		SAVE_FILE_BODY()
	}

	SAVE_JSON_CUSTOM_SIGNATURE(Text)
	{
		format(mText);
		SAVE_JSON_CUSTOM_BODY(Text, IElement)
		unformat(mText);
	}

	LOAD_JSON_CUSTOM_SIGNATURE(Text)
	{
		LOAD_JSON_CUSTOM_BODY(Text, IElement)
		unformat(mText);
	}

	INITIALIZE_CUSTOM_SIGNATURE(Text)
	{
		mOnEnabled += [this] (bool enabled) -> void
		{
			for (const std::unique_ptr<Char>& kChar : mChars)
			{
				if (kChar)
					kChar->display(enabled);
			}
		};

		mFont.onChange([this]() -> void
		{
			makeCharacters();
			if (mFitToSize)
				fitText();
		});
	}

	void Text::draw() const
	{
		if (!mIsEnabled)
			return;

		Pen pen { *this };

		uint32_t charCount { static_cast<uint32_t>(mChars.size()) };
		for (uint32_t i { 0u } ; i < charCount ; ++i)
		{
			// If previous character couldn't be drawn
			if (!pen.mCanDraw)
			{
				if (mChars[i])
					mChars[i]->display(false);

				continue;
			}

			drawCharacter(pen, i);
		}
	}

	void Text::camera(const rhi::IDescriptor* kBuffer)
	{
		if (mWidget->space() != ECoordSpace::WORLD)
			return;

		for (std::unique_ptr<Char>& character : mChars)
		{
			if (!character)
				continue;

			rhi::MaterialInstance* matInst { character->materialInstance() };
			matInst->descriptor(0u, 0u, kBuffer);
			matInst->update(0u, 0u);
		}
	}

	const Font* Text::font() const
	{
		return mFont.get();
	}

	void Text::font(const Font* kFont)
	{
		if (mFont == kFont)
			return;

		if (kFont)
		{
			LOG(LogWarning, "failed to set text font: no font")
			return;
		}

		mFont = kFont;
		makeCharacters();

		if (mFitToSize)
			fitText();
	}

	const std::string& Text::text() const
	{
		return mText;
	}

	void Text::text(const std::string& kStr)
	{
		mText = kStr;
		makeCharacters();

		if (mFitToSize)
			fitText();
	}

	void Text::text(std::string&& str)
	{
		mText = std::move(str);
		makeCharacters();

		if (mFitToSize)
			fitText();
	}

	uint32_t Text::fontSize() const
	{
		return static_cast<uint32_t>(mFontSize * mFont->info().mBitmapSize);
	}

	void Text::fontSize(uint32_t size)
	{
		if (mFitToSize)
			return;

		mFontSize = static_cast<float>(size) / static_cast<float>(mFont->info().mBitmapSize);
	}

	bool Text::fitToSize() const
	{
		return mFitToSize;
	}

	void Text::fitToSize(bool fit)
	{
		if (mFitToSize == fit)
			return;

		mFitToSize = fit;

		if (mFitToSize)
			fitText();
	}

	void Text::resize(const math::Vec2f& kFactors)
	{
		IElement::resize(kFactors);
		for (const std::unique_ptr<Char>& kChar : mChars)
		{
			if (kChar)
				kChar->resize(kFactors);
		}
	}

	void Text::onAttach()
	{
		IElement::onAttach();

		makeCharacters();

		if (mFitToSize)
			fitText();
	}

	void Text::onSpaceChanged(ECoordSpace space)
	{
		const char* kMaterialName { space == ECoordSpace::SCREEN ? "asset/default/screenTextMaterial" : "asset/default/worldTextMaterial" };
		mMaterial = Asset<rhi::Material>::get(kMaterialName);

		mChars.clear();
		makeCharacters();

		if (mFitToSize)
			fitText();
	}

	void Text::onSizeChanged()
	{
		if (mFitToSize)
			fitText();
	}

	void Text::makeCharacters()
	{
		uint32_t count { static_cast<uint32_t>(mText.size()) };
		mChars.resize(count);

		const math::Vec2ui& kRef { mWidget->reference() };
		math::Vec2f screenFactors { static_cast<float>(kRef.mX), static_cast<float>(kRef.mY) };
		screenFactors.inverse();

		for (uint32_t i { 0u } ; i < count ; ++i)
		{
			const Glyph* kGlyph { mFont->glyph(mText[i]) };

			try
			{
				if (mChars[i])
					mChars[i]->glyph(kGlyph, screenFactors);
				else
				{
					mChars[i] = std::make_unique<Char>(mMaterial.get(), kGlyph, screenFactors, mIsEnabled);

					if (mWidget->space() == ECoordSpace::WORLD)
					{
						rhi::MaterialInstance* matInst { mChars[i]->materialInstance() };
						matInst->descriptor(0u, 0u, Service::get<scene::SceneHandler>()->activeScene()->cameras().main()->buffer());
						matInst->update(0u, 0u);

						matInst->descriptor(1u, 0u, mWidget->buffer());
						matInst->update(1u, 0u);
					}
				}
			}
			catch (const std::exception& e)
			{
				LOG(LogWarning, "failed to make characters: character \'" << static_cast<uint32_t>(mText[i]) << "\': " << e.what())
				mChars[i].reset();
			}
		}
	}

	void Text::drawCharacter(Pen& pen, uint32_t charIndex) const
	{
		const std::unique_ptr<Char>& kChar { mChars[charIndex] };

		// Unknown character
		if (!kChar)
		{
			if (mText[charIndex] == '\n')
				pen.newLine();

			return;
		}

		// Check if can be drawn in the text limits
		math::Vec2f charExtent { pen.mPosition + (kChar->size()) * mFontSize };
		pen.testExtent(charExtent);

		kChar->display(pen.mCanDraw);
		if (pen.mCanDraw)
		{
			kChar->draw(pen.mPosition, mFontSize, mColor.data());
			pen.mPosition.mX += kChar->advance() * mFontSize;
		}
	}

	void Text::fitText()
	{
		float lineSize { static_cast<float>(mFont->info().mLineSize) / static_cast<float>(mWidget->reference().mY) };

		uint32_t charCount { static_cast<uint32_t>(mChars.size()) };
		if (charCount == 0u)
		{
			mFontSize = 0.f;
			return;
		}

		math::Vec2f size { 0.f, lineSize };
		float crtAdvance { 0.f };
		for (uint32_t i { 0u } ; i < charCount ; ++i)
		{
			const std::unique_ptr<Char>& kChar { mChars[i] };
			if (!kChar)
			{
				if (mText[i] == '\n')
				{
					size.mY += lineSize;
					if (size.mX < crtAdvance)
						size.mX = crtAdvance;

					crtAdvance = 0.f;
				}
			}
			else
				crtAdvance += kChar->advance();
		}

		if (size.mX < crtAdvance)
			size.mX = crtAdvance;

		mFontSize = std::fminf(mSize.mX / size.mX, mSize.mY / size.mY);
	}
}