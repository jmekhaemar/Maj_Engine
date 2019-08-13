#include "scene/CTransform.h"

#include "scene/SceneHandler.h"

#ifdef MAJ_EDITOR
#include "gizmo/GizmoHandler.h"
#include "gizmo/Handle.h"
#endif

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(CTransform)

	CTransform::CTransform(Actor* actor, bool isStatic)
		: IComponent { actor }, mIsStatic { isStatic }
	{
		initialize();
		onAttach(nullptr);
	}

	CTransform::~CTransform()
	{
#ifdef MAJ_EDITOR
		if (mActor)
			mActor->mOnSelected -= mSelectedEvent;

		Service::get<gizmo::GizmoHandler>()->remove(this);
#endif
	}

	void CTransform::initialize()
	{
#ifdef MAJ_EDITOR
		mHandle = nullptr;
#endif
	}

	void CTransform::moveTo(Actor* actor)
	{
		if (mActor)
		{
			LOG(LogWarning, "failed to set actor: transform cannot be moved")
			return;
		}

		mActor = actor;

		onAttach(nullptr);
	}

	void CTransform::awake()
	{
		mOnChanged();

#ifdef MAJ_EDITOR
		gizmo::GizmoHandler* gizmo { Service::get<gizmo::GizmoHandler>() };
		mHandle = gizmo->add<gizmo::Handle>(this);
		if (!mActor->isSelected())
			gizmo->enable(this, mHandle, false);
#endif
	}

	const math::Transf& CTransform::transform() const
	{
		return mTransform;
	}

	const math::Vec3f& CTransform::position() const
	{
		return mTransform.position();
	}

	const math::Quatf& CTransform::rotation() const
	{
		return mTransform.rotation();
	}

	const math::Vec3f& CTransform::scale() const
	{
		return mTransform.scale();
	}

	void CTransform::transform(const math::Transf& kTrans)
	{
		if (mIsStatic)
		{
			LOG(LogWarning, "failed to set transform: actor \'" << mActor->name() << "\' is static")
			return;
		}

		mTransform = kTrans;
		mOnChanged();
	}

	void CTransform::position(const math::Vec3f& kPos)
	{
		if (mIsStatic)
		{
			LOG(LogWarning, "failed to set position: actor \'" << mActor->name() << "\' is static")
			return;
		}

		mTransform.position(kPos);
		mOnChanged();
	}

	void CTransform::rotation(const math::Quatf& kQuat)
	{
		if (mIsStatic)
		{
			LOG(LogWarning, "failed to set rotation: actor \'" << mActor->name() << "\' is static")
				return;
		}

		mTransform.rotation(kQuat);
		mOnChanged();
	}

	void CTransform::scale(const math::Vec3f& kScale)
	{
		if (mIsStatic)
		{
			LOG(LogWarning, "failed to set scale: actor \'" << mActor->name() << "\' is static")
			return;
		}

		mTransform.scale(kScale);
		mOnChanged();
	}

	math::Transf CTransform::global() const
	{
		const Actor* kParent { mActor->parent() };
		if (!kParent)
			return mTransform;

		const CTransform* kParentTr { kParent->rootTransform() };
		if (!kParentTr)
			return mTransform;

		return kParentTr->global() * mTransform;
	}

	bool CTransform::isStatic() const
	{
		return mIsStatic;
	}

	void CTransform::setStatic(bool isStatic)
	{
		if (mIsStatic == isStatic)
			return;

		mIsStatic = isStatic;

		mOnStaticChanged(static_cast<bool&&>(mIsStatic));
	}
	
	const math::Transf& CTransform::operator->() const
	{
		return mTransform;
	}

	void CTransform::onAttach(Actor* previousActor)
	{
		(void)previousActor;

#ifdef MAJ_EDITOR
		mSelectedEvent = mActor->mOnSelected + [this] (bool selected) -> void
		{
			Service::get<gizmo::GizmoHandler>()->enable(this, mHandle, selected);
		};
#endif
	}
}