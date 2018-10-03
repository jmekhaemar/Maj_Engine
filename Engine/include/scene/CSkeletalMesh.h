#ifndef _CSKELETAL_MESH_H_
#define _CSKELETAL_MESH_H_

#include "scene/ICMesh.h"
#include "resources/SkeletalModel.h"
#include "anim/IAnimationInstance.h"

#define MAX_JOINT_COUNT 100u

namespace maj::scene
{
	class CSkeletalMesh final : public ICMesh
	{
		ENABLE_META_PARENT(CSkeletalMesh, ICMesh)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(CSkeletalMesh)

		CSkeletalMesh(Actor* actor, const rhi::Material* kMaterial = nullptr, const resources::SkeletalModel* kModel = nullptr);
		virtual ~CSkeletalMesh();

		void update(float deltaTime);

		const anim::IAnimationInstance* animation() const;
		void animation(std::nullptr_t null);
		void animation(std::unique_ptr<anim::IAnimationInstance>&& animInstance);
		void animation(uint32_t indexAnim);

		const resources::SkeletalModel* model() const;

	private:
		virtual void onDraw() const override;
		virtual void onAttach(Actor* previousActor) override;

		void drawBindPose() const;

		Asset<resources::SkeletalModel> mModel;
		std::unique_ptr<anim::IAnimationInstance> mCurrentAnim;

		uint32_t mUpdateEventIndex;
	};
}

META_CLASS_PARENT(maj::scene::CSkeletalMesh, maj::scene::ICMesh, 1u)
META_FIELD(maj::scene::CSkeletalMesh, Asset<resources::SkeletalModel>, mModel, 0u)

#endif