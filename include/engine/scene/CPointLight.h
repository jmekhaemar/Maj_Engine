#ifndef _CPOINT_LIGHT_H_
#define _CPOINT_LIGHT_H_

#include "scene/ICLight.h"

namespace maj::scene
{
	class CPointLight final : public ICLight
	{
		ENABLE_META_PARENT(CPointLight, ICLight)

	public:
		LOAD_CONSTRUCTOR(CPointLight)

		CPointLight(Actor* actor, const math::Vec4f& kIntensity = { 1.f, 1.f, 1.f, 1.f }, const Color& kColor = { 255u, 255u, 255u, 255u });
		virtual ~CPointLight() = default;

		virtual void draw(rhi::Buffer::SubBuffer& buffer, uint32_t offset) const override;

		virtual void awake() override;
	};
}

META_CLASS_PARENT(maj::scene::CPointLight, maj::scene::ICLight, 0u)

#endif