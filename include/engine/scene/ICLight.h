#ifndef _ICLIGHT_H_
#define _ICLIGHT_H_

#include <Buffer.h>
#include <Vec4.h>
#include <Vec3.h>

#include "Color.h"

#include "scene/IComponent.h"
#include "scene/Actor.h"

namespace maj::scene
{
	class ICLight : public IComponent
	{
		ENABLE_META_PARENT(ICLight, IComponent)

	public:
		ICLight(Actor* actor, const math::Vec4f& kIntensity = { 1.f, 1.f, 1.f, 2.f }, const Color& kColor = { 255u, 255u, 255u, 255u });
		virtual ~ICLight();

		INITIALIZE_SIGNATURE();

		virtual void draw(rhi::Buffer::SubBuffer& buffer, uint32_t offset) const = 0;

		const Color& color() const;
		void color(const Color& kColor);

		const math::Vec4f& intensity() const;
		void intensity(const math::Vec4f& kIntensity);

	protected:
		ICLight() = default;

		virtual void onAttach(Actor* previousActor) override;

		math::Vec4f mIntensity;
		Color mColor;
	};
}

META_CLASS_PARENT(maj::scene::ICLight, maj::scene::IComponent, 2u)
META_FIELD(maj::scene::ICLight, maj::math::Vec4f, mIntensity, 0u)
META_FIELD(maj::scene::ICLight, maj::Color, mColor, 1u)

#endif