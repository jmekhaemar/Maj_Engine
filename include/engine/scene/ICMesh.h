#ifndef _ICMESH_H_
#define _ICMESH_H_

#include <MaterialInstance.h>
#include <DrawCall.h>

#include "scene/IComponent.h"
#include "scene/Actor.h"

#ifdef MAJ_EDITOR
#include "gizmo/Selection.h"
#endif

namespace maj::scene
{
	class ICMesh : public IComponent
	{
		ENABLE_META_PARENT(ICMesh, IComponent)

	public:
		ICMesh(Actor* actor);
		virtual ~ICMesh();

		INITIALIZE_SIGNATURE();

		void draw() const;
		
		const rhi::MaterialInstance* materialInstance() const;
		rhi::MaterialInstance* materialInstance();
		
		void material(const rhi::Material* kMaterial);

		const rhi::Buffer::SubBuffer* buffer() const;

		virtual void awake() override;

	protected:
		ICMesh();

		virtual void onDraw() const = 0;
		virtual void onAttach(Actor* previousActor) override;

		void addDrawCall(const rhi::DrawCall::Geometry& kGeometry);
		void removeDrawCall();

		rhi::MaterialInstance* mMaterialInstance;
		rhi::Buffer::SubBuffer* mBuffer;

		uint32_t mDrawEventIndex;
		uint32_t mCameraEventIndex;

#ifdef MAJ_EDITOR
		uint32_t mSelectionId;
#endif
	};
}

META_CLASS_PARENT(maj::scene::ICMesh, maj::scene::IComponent, 1u)
META_FIELD(maj::scene::ICMesh, maj::rhi::MaterialInstance*, mMaterialInstance, 0u)

#endif