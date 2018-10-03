#include "scene/CDirectionalLight.h"

#include "scene/SceneHandler.h"
#include "scene/CTransform.h"

#ifdef MAJ_EDITOR
#include "gizmo/Sprite.h"
#endif

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(CDirectionalLight)

	CDirectionalLight::CDirectionalLight(Actor* actor, const math::Vec4f& kIntensity, const Color& kColor)
		: ICLight { actor , kIntensity , kColor }
	{
		initialize();
	}

	void CDirectionalLight::initialize()
	{}

	void CDirectionalLight::draw(rhi::Buffer::SubBuffer& buffer, uint32_t offset) const
	{
		const CTransform* kActorTr { mActor->rootTransform() };
		if (!kActorTr)
		{
			LOG(LogWarning, "failed to draw directional light: actor \'" << mActor->name() << "\' has no root transform")
			return;
		}

		rhi::Buffer::MemoryMap memMap{ buffer };
		math::Transf trans{ mActor->rootTransform()->transform() };

		math::Vec4Holder<float> intensity { mIntensity.mX, mIntensity.mY, mIntensity.mZ, mIntensity.mW };
		math::Vec4Holder<float> pos { trans.forward().mX, trans.forward().mY, trans.forward().mZ, -2.f };
		math::Vec3Holder<float> color{ mColor.mX, mColor.mY, mColor.mZ };

		std::memcpy(memMap.mData + offset, &intensity, sizeof(math::Vec4Holder<float>));
		std::memcpy(memMap.mData + offset + sizeof(math::Vec4Holder<float>), &pos, sizeof(math::Vec4Holder<float>));
		std::memcpy(memMap.mData + offset + sizeof(math::Vec4Holder<float>) * 2, &color, sizeof(math::Vec3Holder<float>));
	}

	void CDirectionalLight::awake()
	{
		ICLight::awake();

#ifdef MAJ_EDITOR
		if (!Asset<rhi::Image>::exists("asset/default/imgDirLight"))
			Asset<rhi::Image>::add("asset/default/imgDirLight", "data/textures/DirectionalLight.png",
								   maj::rhi::Format { maj::rhi::Format::EComponent::R8G8B8A8, maj::rhi::Format::EType::UNORM });

		if (!Asset<rhi::Texture>::exists("asset/default/dirLight"))
			maj::Asset<maj::rhi::Texture>::add("asset/default/dirLight", maj::Asset<maj::rhi::Image>::get("asset/default/imgDirLight"));

		gizmo::GizmoHandler* gizmoHandler { Service::get<gizmo::GizmoHandler>() };
		gizmo::Sprite* sprite { gizmoHandler->add<gizmo::Sprite>(this, Asset<rhi::Texture>::get("asset/default/dirLight")) };
		sprite->color([this]() -> math::Vec3Holder<float> { return { mColor.mX, mColor.mY, mColor.mZ }; });
#endif
	}
}