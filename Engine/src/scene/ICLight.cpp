#include "scene/ICLight.h"

#include "scene/LightHandler.h"
#include "scene/SceneHandler.h"
#include "scene/CStaticMesh.h"

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(ICLight)

	ICLight::ICLight(Actor* actor, const math::Vec4f& kIntensity, const Color& kColor)
		: IComponent { actor }, mIntensity { kIntensity }, mColor { kColor }
	{
		initialize();
		onAttach(nullptr);
	}

	ICLight::~ICLight() 
	{}

	void ICLight::initialize()
	{
		mOnEnabled += [&] () -> void
		{
			Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
			activeScene->lights().add(this);

#ifdef MAJ_EDITOR
			Service::get<gizmo::GizmoHandler>()->enable(this, true);
#endif
		};

		mOnDisabled += [&] (bool destroyed) -> void
		{
			(void)destroyed;

			Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
			activeScene->lights().remove(this);

#ifdef MAJ_EDITOR
			if (destroyed)
				Service::get<gizmo::GizmoHandler>()->remove(this);
			else
				Service::get<gizmo::GizmoHandler>()->enable(this, false);
#endif
		};

		Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		if (mIsEnabled)
			activeScene->lights().add(this);
	}

	const Color& ICLight::color() const
	{
		return mColor;
	}

	void ICLight::color(const Color& kColor)
	{
		mColor = kColor;
	}

	const math::Vec4f& ICLight::intensity() const
	{
		return mIntensity;
	}

	void ICLight::intensity(const math::Vec4f& kIntensity)
	{
		mIntensity = kIntensity;
	}

	void ICLight::onAttach(Actor* previousActor)
	{
		(void)previousActor;
	}
}
