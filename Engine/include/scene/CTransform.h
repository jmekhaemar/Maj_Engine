#ifndef _CTRANFORM_H_
#define _CTRANFORM_H_

#include <Event.h>
#include <Transform.h>

#include "scene/IComponent.h"

#ifdef MAJ_EDITOR
namespace maj::gizmo
{
	class Handle;
}
#endif

namespace maj::scene
{
	class CTransform final : public IComponent
	{
		ENABLE_META_PARENT(CTransform, IComponent)

	public:
		LOAD_CONSTRUCTOR(CTransform)

		CTransform(Actor* actor, bool isStatic = false);
		~CTransform();

		virtual void moveTo(Actor* actor) override;
		virtual void awake() override;

		const math::Transf& transform() const;
		const math::Vec3f& position() const;
		const math::Quatf& rotation() const;
		const math::Vec3f& scale() const;

		void transform(const math::Transf& kTRans);
		void position(const math::Vec3f& kPos);
		void rotation(const math::Quatf& kQuat);
		void scale(const math::Vec3f& kScale);

		math::Transf global() const;

		bool isStatic() const;
		void setStatic(bool isStatic);

		const math::Transf& operator->() const;

		Event<void, bool> mOnStaticChanged;
		Event<void> mOnChanged;

	private:
		virtual void onAttach(Actor* previousActor) override;

		math::Transf mTransform;
		bool mIsStatic;

#ifdef MAJ_EDITOR
		gizmo::Handle* mHandle;
		uint32_t mSelectedEvent;
#endif
	};
}

META_CLASS_PARENT(maj::scene::CTransform, maj::scene::IComponent, 2u)
META_FIELD(maj::scene::CTransform, maj::math::Transf, mTransform, 0u)
META_FIELD(maj::scene::CTransform, bool, mIsStatic, 1u)

#endif