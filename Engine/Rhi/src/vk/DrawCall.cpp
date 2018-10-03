#include "DrawCall.h"

#include "common/Logging.h"

#include "Renderer.h"
#include "MaterialInstance.h"

namespace maj::rhi
{
	bool DrawCall::VertexInput::operator==(const VertexInput& kOther) const
	{
		return mSize == kOther.mSize && mCount == kOther.mCount;
	}

	bool DrawCall::VertexInput::operator!=(const VertexInput& kOther) const
	{
		return mSize != kOther.mSize || mCount != kOther.mCount;
	}

	DrawCall::Geometry::Geometry(const std::vector<VertexInput>& kInputs, uint32_t indexCount, const Buffer::SubBuffer* kData)
		: mVertexInput { kInputs }, mIndexCount { indexCount }, mkData { kData }
	{}

	DrawCall::Geometry::Geometry(std::vector<VertexInput>&& inputs, uint32_t indexCount, const Buffer::SubBuffer* kData)
		: mVertexInput { std::move(inputs) }, mIndexCount { indexCount }, mkData { kData }
	{}

	DrawCall::Geometry::Geometry(const Geometry& kOther)
		: mVertexInput { kOther.mVertexInput }, mIndexCount { kOther.mIndexCount }, mkData { kOther.mkData }
	{}

	DrawCall::Geometry::Geometry(Geometry&& other)
		: mVertexInput { std::move(other.mVertexInput) }, mIndexCount { other.mIndexCount }, mkData { other.mkData }
	{}

	DrawCall::DrawCall(const MaterialInstance* kMaterialInstance, const Geometry& kGeometry, uint32_t instanceCount, uint32_t instanceStart)
		: mkMaterialInstance { kMaterialInstance }, mGeometry { kGeometry }, mInstanceCount { instanceCount }, mInstanceStart { instanceStart }
	{
		if (mInstanceCount == 0u)
			LOG(LogRHIWarning, "failed to create draw call: instance count is null")

		if (mInstanceStart >= mInstanceCount)
			LOG(LogRHIWarning, "failed to create draw call: instance start superior or equal to instance count")
	}

	DrawCall::DrawCall(DrawCall&& other)
		: mkMaterialInstance { other.mkMaterialInstance }, mGeometry { std::move(other.mGeometry) }, mInstanceCount { other.mInstanceCount },
		mInstanceStart { other.mInstanceStart }
	{
		other.mInstanceCount = 0u;
	}

	void DrawCall::record(VkCommandBuffer commandBuffer, bool hasChangedMaterial) const
	{
		// Setup material
		mkMaterialInstance->bind(commandBuffer, hasChangedMaterial);
		
		// Setup geometry & call draw
		try
		{
			recordGeometry(commandBuffer);
		}
		catch (const std::exception& e)
		{
			LOG(LogRHIWarning, e.what())
		}
	}

	const MaterialInstance* DrawCall::materialInstance() const
	{
		return mkMaterialInstance;
	}

	const Material& DrawCall::material() const
	{
		return mkMaterialInstance->material();
	}

	bool DrawCall::operator==(const Geometry& kGeometry) const
	{
		if (mGeometry.mkData)
			return mGeometry.mkData == kGeometry.mkData;

		return !kGeometry.mkData && mGeometry.mIndexCount == kGeometry.mIndexCount && mGeometry.mVertexInput == kGeometry.mVertexInput;
	}

	bool DrawCall::operator!=(const Geometry& kGeometry) const
	{
		if (mGeometry.mkData)
			return mGeometry.mkData != kGeometry.mkData;

		return kGeometry.mkData || mGeometry.mIndexCount != kGeometry.mIndexCount || mGeometry.mVertexInput != kGeometry.mVertexInput;
	}

	void DrawCall::recordGeometry(VkCommandBuffer commandBuffer) const
	{
		if (mInstanceCount == 0u || mInstanceStart >= mInstanceCount)
			THROW("failed to record draw call: invalid instance parameters")
		
		// Compute vertex count & offsets
		uint32_t vertexInputCount { static_cast<uint32_t>(mGeometry.mVertexInput.size()) };
		std::vector<uint64_t> offsets {};
		offsets.reserve(vertexInputCount);

		uint64_t vertexCount { 0u };
		uint64_t indexStart { 0u };

		for (uint32_t i { 0u } ; i < vertexInputCount ; ++i)
		{
			offsets.emplace_back(vertexCount);
			vertexCount += mGeometry.mVertexInput[i].mCount;

			if (mGeometry.mIndexCount > 0u)
				indexStart += mGeometry.mVertexInput[i].mCount * mGeometry.mVertexInput[i].mSize;
		}

		// Bind data
		if (mGeometry.mkData)
		{
			VkBuffer data { mGeometry.mkData->buffer()->buffer() };

			if (vertexInputCount > 0u)
			{
				offsets[0u] += mGeometry.mkData->offset();
				vkCmdBindVertexBuffers(commandBuffer, 0u, vertexInputCount, &data, offsets.data());
			}

			if (mGeometry.mIndexCount > 0u)
				vkCmdBindIndexBuffer(commandBuffer, data, static_cast<uint32_t>(indexStart + mGeometry.mkData->offset()), VK_INDEX_TYPE_UINT32);
		}
		
		// Draw call
		if (mGeometry.mIndexCount > 0u)
			vkCmdDrawIndexed(commandBuffer, mGeometry.mIndexCount, mInstanceCount, 0u, 0u, mInstanceStart);
		else
			vkCmdDraw(commandBuffer, static_cast<uint32_t>(vertexCount), mInstanceCount, 0u, mInstanceStart);
	}
}