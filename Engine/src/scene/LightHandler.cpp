#include "scene/LightHandler.h"

namespace maj::scene
{
	LightHandler::LightHandler()
		: mBuffer { nullptr }, mLights {}
	{}

	LightHandler::~LightHandler()
	{
		rhi::Buffer::release(mBuffer);
	}

	void LightHandler::draw() const
	{
		if (!mBuffer)
			return;

		uint32_t nbLight { static_cast<uint32_t>(mLights.size()) };
		for (uint32_t i { 0u } ; i < nbLight ; ++i)
			mLights[i]->draw(*mBuffer, sizeof(math::Vec4Holder<float>) * 3u * i);
	}

	void LightHandler::add(const ICLight* kLight)
	{
		if (!kLight || find(kLight))
			return;

		uint32_t nbLight { static_cast<uint32_t>(mLights.size()) };
		if (nbLight >= MAX_LIGHT_COUNT)
		{
			LOG(LogWarning, "failed to add light: max light count exceeded")
			return;
		}

		if (!mBuffer)
			mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, (static_cast<uint64_t>(sizeof(math::Vec4Holder<float>) * 3u) * MAX_LIGHT_COUNT + sizeof(uint32_t)));
		
		mLights.emplace_back(kLight);

		{
			++nbLight;
			rhi::Buffer::MemoryMap memMap { *mBuffer };
			std::memcpy(memMap.mData + sizeof(math::Vec4Holder<float>) * 3u * MAX_LIGHT_COUNT, &nbLight, sizeof(uint32_t));
		}
	}

	void LightHandler::remove(const ICLight* kLight)
	{
		if (!kLight)
			return;

		bool found { false };
		for (std::vector<const ICLight*>::const_iterator it { mLights.begin() } ; it != mLights.end() && !found ;)
		{
			if (kLight != *it)
			{
				++it;
				continue;
			}

			it = mLights.erase(it);
			found = true;
		}

		if (!found)
			return;

		{
			uint32_t nbLight { static_cast<uint32_t>(mLights.size()) };
			rhi::Buffer::MemoryMap memMap{ *mBuffer };
			std::memcpy(memMap.mData + sizeof(math::Vec4Holder<float>) * 3u * MAX_LIGHT_COUNT, &nbLight, sizeof(uint32_t));
		}
	}

	const rhi::Buffer::SubBuffer* LightHandler::buffer() const
	{
		return mBuffer;
	}

	bool LightHandler::find(const ICLight* kLight) const
	{
		uint32_t nbLight { static_cast<uint32_t>(mLights.size()) };
		for (uint32_t i { 0u } ; i < nbLight ; ++i)
		{
			if (kLight == mLights[i])
				return true;
		}

		return false;
	}
}