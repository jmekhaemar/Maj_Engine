#include "scene/CPointLight.h"

#include "gizmo/Sprite.h"
#include "scene/SceneHandler.h"
#include "scene/CTransform.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(CPointLight)

	CPointLight::CPointLight(Actor* actor, const math::Vec4f& kIntensity, const Color& kColor)
		:ICLight{ actor , kIntensity , kColor }
	{
		initialize();
	}

	void CPointLight::initialize()
	{}

	void CPointLight::draw(rhi::Buffer::SubBuffer& buffer, uint32_t offset) const
	{
		rhi::Buffer::MemoryMap memMap{ buffer };
		
		const CTransform* kActorTr { mActor->rootTransform() };
		if (kActorTr)
		{
			math::Transf trans{ kActorTr->transform() };

			math::Vec4Holder<float> intensity { mIntensity.data() };
			math::Vec4Holder<float> pos{ trans.position().mX, trans.position().mY, trans.position().mZ, -1.f };
			math::Vec3Holder<float> color{ mColor.mX, mColor.mY, mColor.mZ };

			memcpy(memMap.mData + offset, &intensity, sizeof(math::Vec4Holder<float>));
			memcpy(memMap.mData + offset + sizeof(math::Vec4Holder<float>), &pos, sizeof(math::Vec4Holder<float>));
			memcpy(memMap.mData + offset + sizeof(math::Vec4Holder<float>) * 2, &color, sizeof(math::Vec3Holder<float>));
		}
	}	

	void CPointLight::awake()
	{
		ICLight::awake();

#ifdef MAJ_EDITOR
		if (!Asset<rhi::Image>::exists("asset/default/imgPointLight"))
			Asset<rhi::Image>::add("asset/default/imgPointLight", "data/textures/PointLight.png",
								   maj::rhi::Format { maj::rhi::Format::EComponent::R8G8B8A8, maj::rhi::Format::EType::UNORM });

		if (!Asset<rhi::Texture>::exists("asset/default/pointLight"))
			maj::Asset<maj::rhi::Texture>::add("asset/default/pointLight", maj::Asset<maj::rhi::Image>::get("asset/default/imgPointLight"));

		gizmo::GizmoHandler* gizmoHandler { Service::get<gizmo::GizmoHandler>() };
		gizmo::Sprite* sprite { gizmoHandler->add<gizmo::Sprite>(this, Asset<rhi::Texture>::get("asset/default/pointLight")) };
		
		sprite->color([this]()->math::Vec3Holder<float> { return{ mColor.mX, mColor.mY, mColor.mZ }; });
#endif
	}
}