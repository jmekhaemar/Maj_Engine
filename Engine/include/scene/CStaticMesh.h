#ifndef _CSTATIC_MESH_H_
#define _CSTATIC_MESH_H_

#include "scene/ICMesh.h"
#include "resources/Model.h"

namespace maj::scene
{
	class CStaticMesh final : public ICMesh
	{
		ENABLE_META_PARENT(CStaticMesh, ICMesh)

	public:
		LOAD_CONSTRUCTOR(CStaticMesh)

		CStaticMesh(Actor* actor, const rhi::Material* kMaterial = nullptr, const resources::Model* kModel = nullptr);
		virtual ~CStaticMesh();

		const resources::Model* model() const;

	private:
		virtual void onDraw() const override;
		virtual void onAttach(Actor* previousActor) override;

		Asset<resources::Model> mModel;
	};
}

META_CLASS_PARENT(maj::scene::CStaticMesh, maj::scene::ICMesh, 1u)
META_FIELD(maj::scene::CStaticMesh, maj::Asset<maj::resources::Model>, mModel, 0u)

#endif