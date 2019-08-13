#include "RenderSphere.h"

#include <scene/CStaticMesh.h>

namespace maj::script
{
	DEFINE_HASH_OVERLOAD(RenderSphere)

	RenderSphere::RenderSphere(scene::Actor* actor)
		: scene::CScript { actor }, mBuffer { nullptr }, mColor { 1.f, 0.f, 0.f }, mMetallic { 0.f }, mRoughness { 0.f }
	{
		initialize();
	}

	RenderSphere::~RenderSphere()
	{
		rhi::Buffer::release(mBuffer);
	}

	void RenderSphere::initialize()
	{
		mBuffer = nullptr;
	}

	void RenderSphere::awake()
	{
		CScript::awake();

		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Vec3Holder<float>) + sizeof(float) * 2u);
		if (!mBuffer)
			return;

		{
			rhi::Buffer::MemoryMap memMap { *mBuffer };
			math::Vec3Holder<float> color { mColor.mX, mColor.mY, mColor.mZ };
			std::memcpy(memMap.mData, &color, sizeof(math::Vec3Holder<float>));
			std::memcpy(memMap.mData + sizeof(math::Vec3Holder<float>), &mMetallic, sizeof(float));
			std::memcpy(memMap.mData + sizeof(math::Vec3Holder<float>) + sizeof(float), &mRoughness, sizeof(float));
		}

		scene::CStaticMesh* mesh { mActor->component<scene::CStaticMesh>() };
		if (mesh)
		{
			rhi::MaterialInstance* matInstance { mesh->materialInstance() };
			if (matInstance)
			{
				matInstance->descriptor(1, 1, mBuffer);
				matInstance->update();
			}
		}
	}
}