#include "resources/Model.h"

#include "resources/LoaderObj.h"

namespace maj::resources
{
	DEFINE_HASH_OVERLOAD(Model)

	Model::Model(JsonNode* node)
		: mkPath { "" }
	{
		load(node);
		initialize();
	}
	
	Model::Model(std::vector<Vertex>&& vertices, std::vector<uint32_t>&& indices)
		: mkPath { "" }, mAvailableData { Vertex::EComponentFlag::POSITION | Vertex::EComponentFlag::UV | Vertex::EComponentFlag::NORMAL }, mVertices { std::move(vertices) },
		mIndices { std::move(indices) }, mBuffer { nullptr }
	{
		createBuffer();
	}

	Model::Model(const char* kPath, Vertex::EComponentFlag availableData)
		: mkPath{ kPath }, mAvailableData { availableData }, mVertices{}, mIndices{}, mBuffer{ nullptr }
	{
		initialize();
	}

	Model::~Model()
	{
		rhi::Buffer::release(mBuffer);
	}
	
	void Model::initialize()
	{
		if (mOnChange.empty())
		{
			mOnChange += [this]() -> void
			{
				rhi::Buffer::release(mBuffer);
				mVertices.clear();
				mIndices.clear();

				initialize();
			};
		}

		const char* extension = std::strrchr(mkPath.c_str(), '.');
		if (std::memcmp(extension, ".obj", 4) == 0 || std::memcmp(extension, ".OBJ", 4) == 0)
		{
			try
			{
				Service::get<LoaderObj>()->load(mkPath.c_str(), mVertices, mIndices, mAvailableData);
			}
			catch (const std::exception& e)
			{
				THROW("failed to load model \'" + mkPath +"\': " + std::string{ e.what() })
			}
		}
		else
			THROW("failed to load model \'" + mkPath + "\': extension unsupported (only .obj)")

		createBuffer();
	}

	rhi::DrawCall::Geometry Model::geometry() const
	{
		return { { rhi::DrawCall::VertexInput { sizeof(Vertex), mVertices.size() } },
				static_cast<uint32_t>(mIndices.size()),
				mBuffer };
	}

	void Model::createBuffer()
	{
		uint64_t vertexCount { mVertices.size() }, indexCount { mIndices.size() };
		if (vertexCount == 0u)
			return;

		uint64_t bufferSize { sizeof(mVertices[0u]) * vertexCount + sizeof(mIndices[0]) * indexCount };

		rhi::Buffer::SubBuffer* staging { rhi::Buffer::request(rhi::Buffer::EType::STAGING, bufferSize) };
		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::MODEL, bufferSize);

		{
			rhi::Buffer::MemoryMap memMap{ *staging };
			memcpy(memMap.mData, mVertices.data(), static_cast<size_t>(sizeof(mVertices[0u]) * vertexCount));
			memcpy(memMap.mData + sizeof(mVertices[0u]) * vertexCount, mIndices.data(), static_cast<size_t>(sizeof(mIndices[0]) * indexCount));
		}

		staging->transfer(*mBuffer);
		rhi::Buffer::release(staging);
	}
}