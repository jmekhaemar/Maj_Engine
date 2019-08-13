#include "scene/ICMesh.h"

#include <Renderer.h>

#include "scene/SceneHandler.h"

#ifdef MAJ_EDITOR
#include "gizmo/GizmoHandler.h"
#endif

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(ICMesh)

	ICMesh::ICMesh()
		: IComponent {}, mMaterialInstance { nullptr }, mBuffer { nullptr }, mDrawEventIndex { 0u }, mCameraEventIndex { 0u }
	{}

	ICMesh::ICMesh(Actor* actor)
		: IComponent { actor }, mMaterialInstance { nullptr }, mBuffer { nullptr }, mDrawEventIndex { 0u }, mCameraEventIndex { 0u }
	{
		initialize();
	}

	ICMesh::~ICMesh()
	{
#ifdef MAJ_EDITOR
		Service::get<gizmo::GizmoHandler>()->selection().remove(mSelectionId);
#endif

		if (mMaterialInstance)
		{
			Scene* activeScene { Service::get<SceneHandler>()->activeScene() };
			activeScene->mOnCameraChange -= mCameraEventIndex;

			delete mMaterialInstance;
		}

		rhi::Buffer::release(mBuffer);
	}

	void ICMesh::initialize()
	{
		// Enabled/Disable
		mOnEnabled += [this] () -> void
		{
			mDrawEventIndex = mActor->mOnDraw + std::bind(&ICMesh::draw, this);
		};

		mOnDisabled += [this] (bool destroyed) -> void
		{
			(void)destroyed;

			if (mActor)
				mActor->mOnDraw -= mDrawEventIndex;
		};
	}

	void ICMesh::draw() const
	{
		onDraw();
	}

	const rhi::MaterialInstance* ICMesh::materialInstance() const
	{
		return mMaterialInstance;
	}

	rhi::MaterialInstance* ICMesh::materialInstance()
	{
		return mMaterialInstance;
	}

	void ICMesh::material(const rhi::Material* kMaterial)
	{
		if(mMaterialInstance)
			return;

		mMaterialInstance = new rhi::MaterialInstance { kMaterial };
	}

	const rhi::Buffer::SubBuffer* ICMesh::buffer() const
	{
		return mBuffer;
	}

	void ICMesh::awake()
	{
		IComponent::awake();

		// Scene descriptors
		if (mMaterialInstance)
		{
			Scene* activeScene{ Service::get<SceneHandler>()->activeScene() };
			
			// Camera change
			mCameraEventIndex = activeScene->mOnCameraChange + [this](const rhi::IDescriptor* kBuffer) -> void
			{
				if (mMaterialInstance)
				{
					mMaterialInstance->descriptor(0u, 0u, kBuffer);
					mMaterialInstance->update(0u, 0u);
				}

#ifdef MAJ_EDITOR
				gizmo::Selector* selector { Service::get<gizmo::GizmoHandler>()->selection().selector(mSelectionId) };
				if (selector)
				{
					selector->materialInstance()->descriptor(0u, 0u, kBuffer);
					selector->materialInstance()->update(0u, 0u);
				}
#endif
			};

			scene::CCamera* camera { activeScene->cameras().main() };
			if (camera)
				mMaterialInstance->descriptor(0u, 0u, camera->buffer());

			if (mMaterialInstance->hasDescriptor(0u, 1u))
				mMaterialInstance->descriptor(0u, 1u, activeScene->lights().buffer());

			mMaterialInstance->update();

#ifdef MAJ_EDITOR
			gizmo::Selector* selector { Service::get<gizmo::GizmoHandler>()->selection().selector(mSelectionId) };
			if (selector && camera)
			{
				selector->materialInstance()->descriptor(0u, 0u, camera->buffer());
				selector->materialInstance()->update(0u, 0u);
			}
#endif
		}
	}

	void ICMesh::onAttach(Actor* previousActor)
	{
		if (previousActor)
			previousActor->mOnDraw -= mDrawEventIndex;

		if (mIsEnabled)
			mDrawEventIndex = mActor->mOnDraw + std::bind(&ICMesh::draw, this);
	}

	void ICMesh::addDrawCall(const rhi::DrawCall::Geometry& kGeometry)
	{
		Service::get<rhi::Renderer>()->frameGraph().add(mMaterialInstance, kGeometry);

#ifdef MAJ_EDITOR
		gizmo::Selection& selection { Service::get<gizmo::GizmoHandler>()->selection() };
		mSelectionId = selection.add(mMaterialInstance, kGeometry);
		gizmo::Selector* selector { selection.selector(mSelectionId) };
		if (selector)
		{
			selector->mOnSelected += [this] (bool selected) -> void
			{
				if (mActor)
					Service::get<gizmo::GizmoHandler>()->selectActor(selected ? mActor : nullptr);
			};
		}
#endif
	}

	void ICMesh::removeDrawCall()
	{
		Service::get<rhi::Renderer>()->frameGraph().remove(mMaterialInstance);

#ifdef MAJ_EDITOR
		Service::get<gizmo::GizmoHandler>()->selection().remove(mSelectionId);
#endif
	}
}