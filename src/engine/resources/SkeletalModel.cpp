#include "resources/SkeletalModel.h"

#include "resources/LoaderFbx.h"

namespace maj::resources
{
	DEFINE_HASH_OVERLOAD(SkeletalModel)

	SkeletalModel::SkeletalModel(JsonNode* root)
		: mkPath{ "" }, mBuffer{ nullptr }
	{
		load(root);
		initialize();
	}

	SkeletalModel::SkeletalModel(const char* kPath)
		: mkPath { kPath }, mVertices {}, mIndices{}, mBuffer{ nullptr }
	{
		initialize();
	}

	SkeletalModel::SkeletalModel(SkeletalModel&& other)
		: mVertices{ std::move(other.mVertices) }, mIndices{ std::move(other.mIndices) }, mSkull{ other.mSkull }, mBuffer { other.mBuffer }
	{
		other.mBuffer = nullptr;
	}

	SkeletalModel::~SkeletalModel()
	{
		rhi::Buffer::release(mBuffer);
	}
	
	void SkeletalModel::initialize()
	{
		if (mOnChange.empty())
		{
			mOnChange += [this]() -> void
			{
				rhi::Buffer::release(mBuffer);
				mVertices.clear();
				mIndices.clear();
				mAnimations.clear();
				mSkull.clear();

				initialize();
			};
		}

		const char* extension = std::strrchr(mkPath.c_str(), '.');
		if (std::memcmp(extension, ".fbx", 4) == 0 || std::memcmp(extension, ".FBX", 4) == 0)
		{
			try
			{
				Service::get<LoaderFbx>()->load(mkPath.c_str(), mVertices, mIndices, &mSkull, mAnimations);
			}
			catch (const std::exception& e)
			{
				THROW("failed to load skeletal model \'" + mkPath +"\': " + std::string { e.what() })
			}
		}
		else
			THROW("failed to load skeletal model \'" + mkPath +"\': extension unsupported (only .fbx)")

		createBuffer();
	}

	rhi::DrawCall::Geometry SkeletalModel::geometry() const
	{
		return { { rhi::DrawCall::VertexInput{ sizeof(anim::Vertex), mVertices.size() } },
					static_cast<uint32_t>(mIndices.size()),
					mBuffer };
	}

	const std::vector<uint32_t>& SkeletalModel::indices() const
	{
		return mIndices;
	}

	const anim::Animation& SkeletalModel::animation(const char* kAnimName)const
	{
		for (uint32_t i{ 0u }; i < mAnimations.size(); i++)
		{ 
			if (std::strcmp(mAnimations[i].name().c_str(), kAnimName) == 0)
				return mAnimations[i];

		}
		return  mAnimations[0];
	}

	const anim::Animation& SkeletalModel::animation(uint32_t kAnimIndex)const
	{
		return mAnimations[kAnimIndex];
	}

	const std::vector<anim::Animation>& SkeletalModel::animations()const
	{
		return mAnimations;
	}

	const anim::Skeleton& SkeletalModel::skull()const
	{
		return mSkull;
	}

	void SkeletalModel::createBuffer()
	{
		uint64_t vertexCount { mVertices.size() }, indexCount { mIndices.size() };
		if (vertexCount == 0u || indexCount == 0u)
			return;

		uint64_t bufferSize{ sizeof(mVertices[0u]) * vertexCount + sizeof(mIndices[0]) * indexCount };

		rhi::Buffer::SubBuffer* staging{ rhi::Buffer::request(rhi::Buffer::EType::STAGING, bufferSize) };
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