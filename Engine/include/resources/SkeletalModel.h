#ifndef _SKELETAL_MODEL_H_
#define _SKELETAL_MODEL_H_

#include <Buffer.h>
#include <DrawCall.h>

#include "resources/IModel.h"
#include "anim/AnimVertex.h"
#include "anim/Skeleton.h"
#include "anim/Animation.h"

namespace maj::resources
{
	class SkeletalModel final : public IModel
	{
		ENABLE_META(SkeletalModel)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(SkeletalModel)

		SkeletalModel();
		SkeletalModel(const char* kPath);
		SkeletalModel(SkeletalModel&& other);
		virtual ~SkeletalModel();

		virtual rhi::DrawCall::Geometry geometry() const override;

		const std::vector<uint32_t>& indices() const;

		const anim::Animation& animation(const char* kAnimName)const;
		const anim::Animation& animation(uint32_t kAnimIndex)const;		
		const std::vector<anim::Animation>& animations()const;

		const anim::Skeleton& skull()const;

		Event<void> mOnChange;

	private:
		void createBuffer();

		const std::string mkPath;

		std::vector<anim::Vertex> mVertices;
		std::vector<uint32_t> mIndices;

		anim::Skeleton mSkull;
		std::vector<anim::Animation> mAnimations;
		
		rhi::Buffer::SubBuffer* mBuffer;
	};
}

META_CLASS(maj::Asset<maj::resources::SkeletalModel>, 1u)
META_FIELD(maj::Asset<maj::resources::SkeletalModel>, uint32_t, mGuid, 0u)

META_CLASS(maj::resources::SkeletalModel, 2u)
META_FIELD(maj::resources::SkeletalModel, uint32_t, mGuid, 0u)
META_FIELD(maj::resources::SkeletalModel, const std::string, mkPath, 1u)

#endif
