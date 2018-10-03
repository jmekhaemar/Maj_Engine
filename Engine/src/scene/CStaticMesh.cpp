#include "scene/CStaticMesh.h"

#include <Renderer.h>

#include "scene/CTransform.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(CStaticMesh)

	CStaticMesh::CStaticMesh(Actor* actor, const rhi::Material* kMaterial, const resources::Model* kModel)
		: ICMesh { actor }, mModel { kModel }
	{
		if (!kMaterial)
			material(maj::Asset<maj::rhi::Material>::get("asset/default/objMaterial"));
		else
			material(kMaterial);

		if (!kModel)
			mModel = maj::Asset<resources::Model>::get("asset/default/cube");

		initialize();
		onAttach(nullptr);
	}

	CStaticMesh::~CStaticMesh()
	{}
	
	void CStaticMesh::initialize()
	{
		// Enable/Disable
		mOnEnabled += [&] () -> void
		{
			if (!mModel.empty())
				addDrawCall(mModel->geometry());
		};

		mOnDisabled += [&] (bool destroyed) -> void
		{
			(void)destroyed;
			removeDrawCall();
		};

		// Model
		mModel.onChange([this] () -> void
						{
							removeDrawCall();
							if (!mModel.empty())
								addDrawCall(mModel->geometry());
						});

		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Mat4f));
		mMaterialInstance->descriptor(1u, 0u, mBuffer);
	}

	const resources::Model* CStaticMesh::model() const
	{
		return mModel.get();
	}
	
	void CStaticMesh::onDraw() const
	{
		const CTransform* kActorTr{ mActor->rootTransform() };
		if (!kActorTr)
		{
			LOG(LogWarning, "failed to draw static mesh: actor \'" << mActor->name() << "\' has no root transform")
			return;
		}

		rhi::Buffer::MemoryMap memMap { *mBuffer };
		std::memcpy(memMap.mData, kActorTr->global().matrix().elem(), sizeof(math::Mat4f));
	}

	void CStaticMesh::onAttach(Actor* previousActor)
	{
		ICMesh::onAttach(previousActor);

		if (!previousActor && mIsEnabled && !mModel.empty())
			addDrawCall(mModel->geometry());
	}
}