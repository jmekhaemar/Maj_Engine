#ifndef _MODEL_H_
#define _MODEL_H_

#include <Buffer.h>
#include <DrawCall.h>

#include "resources/IModel.h"
#include "resources/Vertex.h"

namespace maj::resources
{
	class Model final : public IModel
	{
		ENABLE_META(Model)

	public:
		LOAD_CONSTRUCTOR_CUSTOM(Model)
		Model();
		Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices);
		Model(const char* kPath, Vertex::EComponentFlag availableData = Vertex::EComponentFlag::POSITION | Vertex::EComponentFlag::UV | Vertex::EComponentFlag::NORMAL);

		virtual ~Model();

		virtual rhi::DrawCall::Geometry geometry() const override;

		Event<void> mOnChange;

	private:
		void createBuffer();

		const std::string mkPath;
		Vertex::EComponentFlag mAvailableData;

		std::vector<Vertex> mVertices;
		std::vector<uint32_t> mIndices;

		rhi::Buffer::SubBuffer* mBuffer;
	};
}

META_CLASS(maj::Asset<maj::resources::Model>, 1u)
META_FIELD(maj::Asset<maj::resources::Model>, uint32_t, mGuid, 0u)

META_CLASS(maj::resources::Model, 3u)
META_FIELD(maj::resources::Model, uint32_t, mGuid, 0u)
META_FIELD(maj::resources::Model, const std::string, mkPath, 1u)
META_FIELD(maj::resources::Model, maj::resources::Vertex::EComponentFlag, mAvailableData, 2u)

#endif
