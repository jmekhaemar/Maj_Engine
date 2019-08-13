#include "Light.h"

#include <Input.h>
#include <scene/ICLight.h>

namespace maj::script
{
	DEFINE_HASH_OVERLOAD(Light)

	Light::Light(JsonNode* root)
		: scene::CScript {}, mTime { 0.f }
	{
		load(root);
		initialize();
	}

	Light::Light(scene::Actor* actor)
		: scene::CScript { actor }, mTime { 0.f }
	{
		initialize();
	}

	void Light::initialize()
	{
		mTime = 0.f;
	}

	void Light::update(float deltaTime)
	{
		mTime += deltaTime * 4.0f;
		scene::ICLight* light { mActor->component<scene::ICLight>() };
		if (light)
		{
			math::Vec4f intensity { light->intensity() };
			intensity.mW = std::cos(mTime) + 1.5f;
			light->intensity(intensity);
		}
	}
}