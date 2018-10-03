#include "gui/Char.h"

#include <Renderer.h>

namespace maj::gui
{
	Char::Char(const rhi::Material* kMaterial, const Glyph* kGlyph, const math::Vec2f& kScreenFactor, bool displayed)
		: mMaterialInst { nullptr }, mBuffer { nullptr }, mIsDisplayed { displayed }, mkGlyph { kGlyph }, mSize {}, mBearing {}, mAdvance { 0.f }
	{
		if (!mkGlyph)
			THROW("failed to create text character: no glyph")

		retrieveGlyphInfo(kScreenFactor);

		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Vec2Holder<float>) * 2u + sizeof(math::Vec4Holder<float>));
		material(kMaterial);
	}

	Char::Char(Char&& other)
		: mMaterialInst { other.mMaterialInst }, mBuffer { other.mBuffer }, mIsDisplayed { other.mIsDisplayed }, mkGlyph { other.mkGlyph }, mSize { other.mSize },
		mBearing { other.mBearing }, mAdvance { other.mAdvance }
	{
		other.mMaterialInst = nullptr;
		other.mBuffer = nullptr;
	}

	Char::~Char()
	{
		if (mMaterialInst)
			delete mMaterialInst;

		rhi::Buffer::release(mBuffer);
	}

	void Char::material(const rhi::Material* kMaterial)
	{
		if (mMaterialInst)
			delete mMaterialInst;

		mMaterialInst = new rhi::MaterialInstance { kMaterial };
		mMaterialInst->descriptor(2u, 0u, mBuffer);

		// Check for bitmap, if none then there is no need to draw anything
		const rhi::Texture* kBitmap { mkGlyph->bitmap() };
		if (kBitmap)
		{
			mMaterialInst->descriptor(2u, 1u, kBitmap);
			mMaterialInst->update(2u);

			if (mIsDisplayed)
			{
				rhi::Renderer* renderer { Service::get<rhi::Renderer>() };
				rhi::DrawCall::Geometry quad { { rhi::DrawCall::VertexInput { 1u, 4u } } };
				renderer->frameGraph().add(mMaterialInst, std::move(quad));
			}
		}
	}

	const rhi::MaterialInstance* Char::materialInstance() const
	{
		return mMaterialInst;
	}

	rhi::MaterialInstance* Char::materialInstance()
	{
		return mMaterialInst;
	}

	const Glyph* Char::glyph() const
	{
		return mkGlyph;
	}

	void Char::glyph(const Glyph* kGlyph, const math::Vec2f& kScreenFactor)
	{
		if (mkGlyph == kGlyph)
			return;

		if (!kGlyph)
		{
			LOG(LogWarning, "failed to set text character: no glyph")
			return;
		}

		bool hadBitmap { mkGlyph->bitmap() != nullptr };

		mkGlyph = kGlyph;
		retrieveGlyphInfo(kScreenFactor);

		// Check for bitmap, if none then there is no need to draw anything
		const rhi::Texture* kBitmap { mkGlyph->bitmap() };
		rhi::Renderer* renderer { Service::get<rhi::Renderer>() };

		if (!kBitmap)
		{
			renderer->frameGraph().remove(mMaterialInst);
			return;
		}

		mMaterialInst->descriptor(2u, 1u, kBitmap);
		mMaterialInst->update(2u, 1u);

		if (!hadBitmap && mIsDisplayed)
		{
			rhi::DrawCall::Geometry quad { { rhi::DrawCall::VertexInput { 1u, 4u } } };
			renderer->frameGraph().add(mMaterialInst, std::move(quad));
		}
	}

	const math::Vec2f& Char::bearing() const
	{
		return mBearing;
	}

	const math::Vec2f& Char::size() const
	{
		return mSize;
	}

	float Char::advance() const
	{
		return mAdvance;
	}

	bool Char::isDisplayed() const
	{
		return mIsDisplayed;
	}

	void Char::display(bool displayed)
	{
		if (displayed == mIsDisplayed)
			return;

		mIsDisplayed = displayed;

		// Check for bitmap, if none then there is no need to draw anything
		const rhi::Texture* kBitmap { mkGlyph->bitmap() };

		rhi::Renderer* renderer { Service::get<rhi::Renderer>() };
		if (displayed && kBitmap)
		{
			rhi::DrawCall::Geometry quad { { rhi::DrawCall::VertexInput { 1u, 4u } } };
			renderer->frameGraph().add(mMaterialInst, std::move(quad));
		}
		else
			renderer->frameGraph().remove(mMaterialInst);
	}

	void Char::draw(const math::Vec2f& kPen, float fontSize, const math::Vec4Holder<float>& kColor) const
	{
		math::Vec2Holder<float> size { (mSize * fontSize).data() };
		math::Vec2Holder<float> pos { (mBearing * fontSize + kPen).data() };

		rhi::Buffer::MemoryMap memMap { *mBuffer };
		std::memcpy(memMap.mData, &pos, sizeof(math::Vec2Holder<float>));
		std::memcpy(memMap.mData + sizeof(math::Vec2Holder<float>), &size, sizeof(math::Vec2Holder<float>));
		std::memcpy(memMap.mData + (2 * sizeof(math::Vec2Holder<float>)), &kColor, sizeof(math::Vec4Holder<float>));
	}

	void Char::resize(const math::Vec2f& kScale)
	{
		mSize = mSize.scaled(kScale);
		mBearing = mBearing.scaled(kScale);
		mAdvance *= kScale.mX;
	}

	void Char::retrieveGlyphInfo(const math::Vec2f& kScreenFactor)
	{
		const Glyph::Info& kInfo { mkGlyph->info() };
		mSize.mX = static_cast<float>(kInfo.mSize.mX) * kScreenFactor.mX;
		mSize.mY = static_cast<float>(kInfo.mSize.mY) * kScreenFactor.mY;

		mBearing.mX = static_cast<float>(kInfo.mBearing.mX) * kScreenFactor.mX;
		mBearing.mY = static_cast<float>(kInfo.mBearing.mY) * kScreenFactor.mY;

		mAdvance = static_cast<float>(kInfo.mAdvance) * kScreenFactor.mX;
	}
}