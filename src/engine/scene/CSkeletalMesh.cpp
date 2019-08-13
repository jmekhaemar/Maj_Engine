#include "scene/CSkeletalMesh.h"

#include <FrameGraph.h>

#include "scene/CTransform.h"
#include "anim/AnimationInstance.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(CSkeletalMesh)

	CSkeletalMesh::CSkeletalMesh(JsonNode* root)
		: ICMesh {}, mModel {}, mCurrentAnim { nullptr }, mUpdateEventIndex { 0u }
	{
		load(root);
		initialize();
	}

	CSkeletalMesh::CSkeletalMesh(Actor* actor, const rhi::Material* kMaterial, const resources::SkeletalModel* kModel)
		:ICMesh { actor}, mModel { kModel }, mCurrentAnim { nullptr }, mUpdateEventIndex { 0u }
	{
		if (!kMaterial)
			material(Asset<maj::rhi::Material>::get("asset/default/FbxMaterial"));
		else
			material(kMaterial);

		if(!kModel)
			mModel = maj::Asset<resources::SkeletalModel>::get("asset/default/cubeFbx");

		initialize();
		onAttach(nullptr);
	}

	CSkeletalMesh::~CSkeletalMesh()
	{}
	
	void CSkeletalMesh::initialize()
	{
		// Enable/Disable
		mOnEnabled += [&]() -> void
		{
			if (!mModel.empty())
				addDrawCall(mModel->geometry());

			mUpdateEventIndex = mActor->mOnUpdate + std::bind(&CSkeletalMesh::update, this, std::placeholders::_1);
		};
		mOnDisabled += [&](bool destroyed) -> void
		{
			(void)destroyed;

			removeDrawCall();
			mActor->mOnUpdate -= mUpdateEventIndex;
		};

		// Model
		mModel.onChange([this] () -> void
						{
							removeDrawCall();
							if (!mModel.empty())
								addDrawCall(mModel->geometry());

							drawBindPose();
						});

		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Mat4f) * (MAX_JOINT_COUNT + 1u) + sizeof(uint32_t));
		drawBindPose();

		mMaterialInstance->descriptor(1u, 0u, mBuffer);
	}

	void CSkeletalMesh::update(float deltaTime)
	{
		if (mModel.empty())
			return;

		uint32_t jointCount { mModel->skull().jointCount() };
		if (jointCount == 0u || !mCurrentAnim)
			return;

		mCurrentAnim->update(deltaTime);

		rhi::Buffer::MemoryMap map { *mBuffer };
		for (uint32_t jointIndex { 0u } ; jointIndex < jointCount && jointIndex < MAX_JOINT_COUNT ; ++jointIndex)
		{
			std::memcpy(map.mData + (jointIndex + 1u) * sizeof(math::Mat4f), (mCurrentAnim->transform(jointIndex) * mModel->skull().joint(jointIndex).mGlobalBind).matrix().elem(),
						sizeof(math::Mat4f));
		}

		std::memcpy(map.mData + sizeof(math::Mat4f) * (MAX_JOINT_COUNT + 1u), &jointCount, sizeof(uint32_t));
	}

	const anim::IAnimationInstance* CSkeletalMesh::animation()const
	{
		return mCurrentAnim.get();
	}

	void CSkeletalMesh::animation(std::nullptr_t null)
	{
		(void) null;

		mCurrentAnim = nullptr;
		drawBindPose();
	}

	void CSkeletalMesh::animation(std::unique_ptr<anim::IAnimationInstance>&& animInstance)
	{
		if (mCurrentAnim == animInstance)
			return;

		if (!animInstance)
		{
			LOG(LogWarning, "failed to set animation: no animation instance")
			return;
		}

		mCurrentAnim = std::move(animInstance);
	}

	void CSkeletalMesh::animation(uint32_t indexAnim)
	{
		uint32_t animCount { static_cast<uint32_t>(mModel->animations().size()) };
		if (indexAnim >= animCount)
		{
			LOG(LogWarning, "failed to set animation: index out of range")
			return;
		}

		mCurrentAnim = std::make_unique<anim::AnimationInstance>(mModel->animation(indexAnim));
	}

	const resources::SkeletalModel* CSkeletalMesh::model() const
	{
		return mModel.get();
	}
	
	void CSkeletalMesh::onDraw() const
	{
		const CTransform* kActorTr { mActor->rootTransform() };
		if (!kActorTr)
		{
			LOG(LogWarning, "failed to draw skeletal mesh: actor \'" << mActor->name() << "\' has no root transform")
			return;
		}

		rhi::Buffer::MemoryMap memMap { *mBuffer };
		std::memcpy(memMap.mData, kActorTr->global().matrix().elem(), sizeof(math::Mat4f));
	}

	void CSkeletalMesh::onAttach(Actor* previousActor)
	{
		ICMesh::onAttach(previousActor);

		if (previousActor)
			previousActor->mOnUpdate -= mUpdateEventIndex;
		else if (mIsEnabled && !mModel.empty())
			addDrawCall(mModel->geometry());

		if (mIsEnabled)
			mUpdateEventIndex = mActor->mOnUpdate + std::bind(&CSkeletalMesh::update, this, std::placeholders::_1);
	}

	void CSkeletalMesh::drawBindPose() const
	{
		if (mModel.empty())
			return;

		rhi::Buffer::MemoryMap map { *mBuffer };
		uint32_t jointCount { mModel->skull().jointCount() };
		for (uint32_t jointIndex { 0u } ; jointIndex < jointCount && jointIndex < MAX_JOINT_COUNT ; ++jointIndex)
			std::memcpy(map.mData + (jointIndex + 1u) * sizeof(math::Mat4f), math::Mat4f::skIdentity.elem(), sizeof(math::Mat4f));

		std::memcpy(map.mData + sizeof(math::Mat4f) * (MAX_JOINT_COUNT + 1u), &jointCount, sizeof(uint32_t));
	}
}
