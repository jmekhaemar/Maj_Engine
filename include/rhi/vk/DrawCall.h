#ifndef _DRAW_CALL_H_
#define _DRAW_CALL_H_

#include <vector>

#include "Material.h"
#include "RenderPass.h"
#include "Buffer.h"

namespace maj::rhi
{
	class MaterialInstance;

	class DrawCall final
	{
	public:
		struct VertexInput final
		{
			bool operator==(const VertexInput& kOther) const;
			bool operator!=(const VertexInput& kOther) const;

			uint64_t mSize;		// Size of each vertex
			uint64_t mCount;	// Number of vertex
		};

		struct Geometry final
		{
		public:
			Geometry(const std::vector<VertexInput>& kInputs = {}, uint32_t indexCount = 0u, const Buffer::SubBuffer* kData = nullptr);
			Geometry(std::vector<VertexInput>&& inputs, uint32_t indexCount = 0u, const Buffer::SubBuffer* kData = nullptr);
			Geometry(const Geometry& other);
			Geometry(Geometry&& other);
			~Geometry() = default;

			// TODO operator= copy/move

			std::vector<VertexInput> mVertexInput;
			uint32_t mIndexCount;
			const Buffer::SubBuffer* mkData;
		};

	public:
		DrawCall(const MaterialInstance* kMaterialInstance, const Geometry& kGeometry, uint32_t instanceCount = 1u, uint32_t instanceStart = 0u);
		DrawCall(DrawCall&& other);
		~DrawCall() = default;

		void record(VkCommandBuffer commandBuffer, bool hasChangedMaterial) const;

		const MaterialInstance* materialInstance() const;
		const Material& material() const;

		bool operator==(const Geometry& kGeometry) const;
		bool operator!=(const Geometry& kGeometry) const;

	private:
		void recordGeometry(VkCommandBuffer commandBuffer) const;

		const MaterialInstance* mkMaterialInstance;
		Geometry mGeometry;

		uint32_t mInstanceCount;
		uint32_t mInstanceStart;
	};
}

#endif