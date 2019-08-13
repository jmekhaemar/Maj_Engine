#ifndef _ASKYBOX_H_
#define _ASKYBOX_H_

#include <MaterialInstance.h>

#include "scene/Actor.h"

namespace maj::scene
{
	class ASkybox final : public Actor
	{
		ENABLE_META_PARENT(ASkybox, Actor)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(ASkybox)

		ASkybox(const std::string& kName, const rhi::Material* kMaterial);
		~ASkybox();

		const rhi::MaterialInstance* material() const;
		rhi::MaterialInstance* material();
		void material(const rhi::Material* kMaterial);

		virtual void awake() override;

	private:
		rhi::MaterialInstance* mMaterialInstance;

		uint32_t mCameraEventIndex;
	};
}

META_CLASS_PARENT(maj::scene::ASkybox, maj::scene::Actor, 1u)
META_FIELD(maj::scene::ASkybox, maj::rhi::MaterialInstance*, mMaterialInstance, 0u)

#endif