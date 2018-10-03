#ifndef _CDIRECTIONAL_LIGHT_H_
#define _CDIRECTIONAL_LIGHT_H_

#include "scene/ICLight.h"

namespace maj::scene
{
	class CDirectionalLight final : public ICLight
	{
		ENABLE_META_PARENT(CDirectionalLight, ICLight)

	public:
		LOAD_CONSTRUCTOR(CDirectionalLight)

		CDirectionalLight(Actor* actor, const math::Vec4f& kIntensity = { 1.f, 1.f, 1.f, 1.f }, const Color& kColor = { 255u, 255u, 255u, 255u });
		virtual ~CDirectionalLight() = default;
		
		virtual void draw(rhi::Buffer::SubBuffer& buffer, uint32_t offset) const override;

		virtual void awake() override;
	};
}

META_CLASS_PARENT(maj::scene::CDirectionalLight, maj::scene::ICLight, 0u)

#endif