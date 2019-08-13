#ifndef _SELECTOR_H_
#define _SELECTOR_H_

#include <NonCopyable.h>
#include <Event.h>
#include <DrawCall.h>
#include <MaterialInstance.h>

namespace maj::gizmo
{
	class Selector final : public NonCopyable
	{
	private:
		union Id
		{
			uint32_t id;
			uint8_t rgba[4u];
		};

	public:
		Selector(uint32_t id, const rhi::MaterialInstance* kMaterialInstance, const rhi::RenderPass& kRenderPass, const rhi::DrawCall::Geometry& kGeometry);
		~Selector();

		const rhi::MaterialInstance* materialInstance() const;
		rhi::MaterialInstance* materialInstance();

		Event<void, bool> mOnSelected;

	private:
		std::unique_ptr<rhi::MaterialLayout> mLayout;
		std::unique_ptr<rhi::Material> mMaterial;

		std::unique_ptr<rhi::MaterialInstance> mMaterialInstance;
		rhi::DrawCall::Geometry mGeometry;
		rhi::Buffer::SubBuffer* mBuffer;
	};
}

#endif