#include "FrameGraph.h"

#include "common/Logging.h"
#include "Renderer.h"
#include "MaterialInstance.h"

namespace maj::rhi
{
	const uint32_t FrameGraph::Key::skMaxMaterialIndex { 1u << MATERIAL_INDEX_BIT };
	const uint32_t FrameGraph::Key::skMaxSortingOrder { 1u << SORTING_ORDER_BIT };
	const uint32_t FrameGraph::Key::skMaxSubpassIndex { 1u << SUBPASS_INDEX_BIT };
	const uint32_t FrameGraph::Key::skMaxRenderPassIndex { 1u << RENDERPASS_INDEX_BIT };

	std::unordered_map<uint32_t, FrameGraph::RenderPassRef> FrameGraph::sRenderPasses {};
	uint32_t FrameGraph::sMaterialCount { 0u };

	FrameGraph::Key::operator uint32_t() const
	{
		return	mMaterialIndex +
			(mSortingOrder << MATERIAL_INDEX_BIT) +
			(mIsScreenSpace << (MATERIAL_INDEX_BIT + SORTING_ORDER_BIT)) +
			(mSubpassIndex << (MATERIAL_INDEX_BIT + SORTING_ORDER_BIT + 1u)) +
			(mRenderPassIndex << (MATERIAL_INDEX_BIT + SORTING_ORDER_BIT + 1u + SUBPASS_INDEX_BIT));
	}

	FrameGraph::CommandBuffer::CommandBuffer(VkCommandBuffer buffer)
		: mBuffer { buffer }, mIsDirty { true }
	{}

	FrameGraph::CommandBuffer::CommandBuffer(CommandBuffer&& other)
		: mBuffer { other.mBuffer }, mIsDirty { other.mIsDirty }
	{
		other.mBuffer = VK_NULL_HANDLE;
		other.mIsDirty = true;
	}

	FrameGraph::RenderPassRef::RenderPassRef(const RenderPass& kRenderPass)
		: mkRenderPass { &kRenderPass }, mMaterials {}
	{}

	FrameGraph::RenderPassRef::~RenderPassRef()
	{
		sMaterialCount -= static_cast<uint32_t>(mMaterials.size());
	}

	uint32_t FrameGraph::RenderPassRef::materialIndex(const Material& kMaterial, bool addIfNone)
	{
		uint32_t materialIndex { Key::skMaxMaterialIndex };
		static uint32_t sMaterialMaxId { 0u };

		// Find if already used
		uint32_t freeIndex { Key::skMaxMaterialIndex };
		for (uint32_t i { 0u } ; i < sMaterialMaxId && materialIndex == Key::skMaxMaterialIndex ; ++i)
		{
			if (!mMaterials.count(i))
			{
				if (freeIndex == Key::skMaxMaterialIndex)
					freeIndex = i;
			}
			else if (mMaterials.at(i) == &kMaterial)
				materialIndex = i;
		}

		// If found or not found but shouldn't be added, return
		if (materialIndex != Key::skMaxMaterialIndex || !addIfNone)
			return materialIndex;

		// If exceeded size, error
		if (sMaterialMaxId >= Key::skMaxMaterialIndex - 1u)
			THROW("failed to retrieve material index: exceeded max size");

		// Add it where it's available
		if (freeIndex != Key::skMaxMaterialIndex)
		{
			mMaterials.emplace(freeIndex, &kMaterial);
			++sMaterialCount;
			return freeIndex;
		}

		mMaterials.emplace(sMaterialMaxId, &kMaterial);
		++sMaterialCount;
		return sMaterialMaxId++;
	}

	uint32_t FrameGraph::renderPassIndex(const RenderPass& kRenderPass, bool addIfNone)
	{
		uint32_t renderPassIndex { Key::skMaxRenderPassIndex };
		static uint32_t sRenderPassMaxId { 0u };

		// Find if already used
		uint32_t freeIndex { Key::skMaxRenderPassIndex };
		for (uint32_t i { 0u } ; i < sRenderPassMaxId && renderPassIndex == Key::skMaxRenderPassIndex ; ++i)
		{
			if (!sRenderPasses.count(i))
			{
				if (freeIndex == Key::skMaxRenderPassIndex)
					freeIndex = i;
			}
			else if (sRenderPasses.at(i).mkRenderPass == &kRenderPass)
				renderPassIndex = i;
		}

		// If found or not found but shouldn't be added, return
		if (renderPassIndex != Key::skMaxRenderPassIndex || !addIfNone)
			return renderPassIndex;

		// If exceeded size, error
		if (sRenderPassMaxId >= Key::skMaxRenderPassIndex - 1u)
			THROW("failed to retrieve render pass index: exceeded max size");

		// Add it where it's available
		if (freeIndex != Key::skMaxRenderPassIndex)
		{
			sRenderPasses.try_emplace(freeIndex, kRenderPass);
			return freeIndex;
		}

		sRenderPasses.try_emplace(sRenderPassMaxId, kRenderPass);
		return sRenderPassMaxId++;
	}

	uint32_t FrameGraph::materialIndex(const Material& kMaterial, uint32_t renderPassIndex, bool addIfNone)
	{
		// If the render pass isn't registered
		if (sRenderPasses.count(renderPassIndex) == 0u)
			return Key::skMaxMaterialIndex;

		return sRenderPasses.at(renderPassIndex).materialIndex(kMaterial, addIfNone);
	}

	FrameGraph::FrameGraph()
		: mDrawCalls {}, mCommands {}, mIsDirty { true }
	{}

	FrameGraph::~FrameGraph()
	{
		clear();
	}

	void FrameGraph::clear()
	{
		Service::get<Renderer>()->wait();

		uint32_t commandCount{ static_cast<uint32_t>(mCommands.size()) };
		if (commandCount == 0u)
			return;

		std::vector<VkCommandBuffer> buffers{};
		buffers.reserve(commandCount);

		for (std::map<Key, CommandBuffer>::const_iterator it{ mCommands.begin() }; it != mCommands.end(); ++it)
			buffers.emplace_back(it->second.mBuffer);

		vkFreeCommandBuffers(Service::get<Device>()->device(), Service::get<Renderer>()->commandPool(), commandCount, buffers.data());
		mCommands.clear();
		mDrawCalls.clear();
		sRenderPasses.clear();
	}

	bool FrameGraph::isDirty() const
	{
		return mIsDirty;
	}

	void FrameGraph::dirty(const RenderPass& kRenderPass)
	{
		mIsDirty = true;
		uint32_t renderPassIdx { renderPassIndex(kRenderPass, false) };

		std::map<Key, CommandBuffer>::iterator low { mCommands.lower_bound({ 0u, 0u, false, 0u, renderPassIdx }) };
		std::map<Key, CommandBuffer>::iterator up { mCommands.upper_bound({ Key::skMaxMaterialIndex - 1u, Key::skMaxSortingOrder - 1u, true, Key::skMaxSubpassIndex - 1u, renderPassIdx }) };

		if (low == mCommands.end())
			return;

		for (std::map<Key, CommandBuffer>::iterator it { low } ; it != up ; ++it)
			it->second.mIsDirty = true;
	}

	void FrameGraph::dirty(const Material* kMaterial)
	{
		if (!kMaterial)
			return;

		const Material::Info& kNewInfo { kMaterial->info() };

		uint32_t renderPassIdx { renderPassIndex(kNewInfo.renderPass(), false) };
		uint32_t materialIdx { materialIndex(*kMaterial, renderPassIdx, false) };

		Key newKey { materialIdx, kNewInfo.mSortingOrder, !kNewInfo.mPipelineState.mDepthTestEnabled, kNewInfo.mSubpassIndex, renderPassIdx };
		replaceDrawCallKey(newKey);
		replaceCommandBufferKey(newKey);
	}

	void FrameGraph::dirty(const Material& kMaterial)
	{
		const Material::Info& kInfo { kMaterial.info() };

		uint32_t renderPassIdx { renderPassIndex(kInfo.renderPass(), false) };
		uint32_t materialIdx { materialIndex(kMaterial, renderPassIdx, false) };

		Key key { materialIdx, kInfo.mSortingOrder, !kInfo.mPipelineState.mDepthTestEnabled, kInfo.mSubpassIndex, renderPassIdx };
		if (mCommands.count(key) == 0u)
			return;

		mCommands.at(key).mIsDirty = true;
		mIsDirty = true;
	}

	void FrameGraph::record()
	{
		// Check if up to date
		if (!mIsDirty)
			return;

		Service::get<Renderer>()->wait();

		// Check if enough commands to record
		// Each material is bound with one subpass of a particular render pass, so we only need as much command buffer as there are materials
		uint32_t commandCount { static_cast<uint32_t>(mCommands.size()) };
		if (commandCount < sMaterialCount)
			createCommands(sMaterialCount - commandCount);

		// For each command buffer
		using DrawCallRange = std::pair<std::multimap<Key, DrawCall>::const_iterator, std::multimap<Key, DrawCall>::const_iterator>;
		for (std::map<Key, CommandBuffer>::iterator commandIt { mCommands.begin() } ; commandIt != mCommands.end() ;)
		{
			// Check if the command buffer needs to be re-recorded
			if (!commandIt->second.mIsDirty)
			{
				++commandIt;
				continue;
			}

			// Find range of draw calls, if none then no draw call for this material
			DrawCallRange drawCallRange { mDrawCalls.equal_range(commandIt->first) };
			if (drawCallRange.first == mDrawCalls.end() || sRenderPasses.count(commandIt->first.mRenderPassIndex) == 0u)
			{
				vkFreeCommandBuffers(Service::get<Device>()->device(), Service::get<Renderer>()->commandPool(), 1u, &commandIt->second.mBuffer);
				commandIt = mCommands.erase(commandIt);
				continue;
			}

			const RenderPass* kRenderPass { sRenderPasses.at(commandIt->first.mRenderPassIndex).mkRenderPass };

			// Start new buffer
			VkCommandBufferInheritanceInfo inheritanceInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO };
			inheritanceInfo.renderPass = kRenderPass->renderPass();
			inheritanceInfo.subpass = commandIt->first.mSubpassIndex;
			inheritanceInfo.framebuffer = VK_NULL_HANDLE;

			// Setup command buffer
			VkCommandBufferBeginInfo beginInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT | VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
			beginInfo.pInheritanceInfo = &inheritanceInfo;

			vkBeginCommandBuffer(commandIt->second.mBuffer, &beginInfo);

			// Record every draw call
			for (std::multimap<Key, DrawCall>::const_iterator drawCallIt { drawCallRange.first } ; drawCallIt != drawCallRange.second ; ++drawCallIt)
				drawCallIt->second.record(commandIt->second.mBuffer, drawCallIt == drawCallRange.first);

			// End previous buffer if any
			VkResult result { vkEndCommandBuffer(commandIt->second.mBuffer) };
			if (result != VK_SUCCESS)
				THROW("failed to record draw call with error: " + std::to_string(result))

			commandIt->second.mIsDirty = false;
			++commandIt;
		}

		mIsDirty = false;
		mOnRecorded();
	}

	void FrameGraph::remove(const MaterialInstance* kMaterialInstance)
	{
		if (!kMaterialInstance)
		{
			LOG(LogRHIWarning, "failed to remove draw call: no material instance")
			return;
		}

		// Make search key
		const Material& kMaterial { kMaterialInstance->material() };
		const Material::Info& kInfo { kMaterial.info() };

		uint32_t renderPassIdx { renderPassIndex(kInfo.renderPass(), false) };
		uint32_t materialIdx { materialIndex(kMaterial, renderPassIdx, false) };
		uint32_t subpassIdx { kInfo.mSubpassIndex };

		if (renderPassIdx == Key::skMaxRenderPassIndex || materialIdx == Key::skMaxMaterialIndex)
			return;

		Key key { materialIdx, kInfo.mSortingOrder, !kInfo.mPipelineState.mDepthTestEnabled, subpassIdx, renderPassIdx };

		// Finding draw calls with this key
		Key upKey { key };
		upKey.mMaterialIndex += 1u;

		std::multimap<Key, DrawCall>::const_iterator kLow { mDrawCalls.lower_bound(key) };
		std::multimap<Key, DrawCall>::const_iterator kUp { mDrawCalls.upper_bound(upKey) };
		
		if (kLow == mDrawCalls.end())
			return;

		// Search draw calls with this key to remove the one corresponding
		for (std::multimap<Key, DrawCall>::const_iterator it { kLow } ; it != kUp ;)
		{
			if (it->second.materialInstance() != kMaterialInstance)
				++it;
			else
				it = mDrawCalls.erase(it);
		}

		// If there is no command buffer for this material, even through it has a draw call, it means the draw call was just added
		if (mCommands.count(key) == 0u)
			return;

		mCommands.at(key).mIsDirty = true;
		mIsDirty = true;
	}

	void FrameGraph::remove(const RenderPass& kRenderPass)
	{
		uint32_t renderPassIdx { renderPassIndex(kRenderPass, false) };
		if (sRenderPasses.count(renderPassIdx) == 0u)
			return;

		// Finding draw calls with this key
		Key lowKey { 0u, 0u, false, 0u, renderPassIdx };
		Key upKey { Key::skMaxMaterialIndex - 1u, Key::skMaxSubpassIndex - 1u, true, Key::skMaxSubpassIndex - 1u, renderPassIdx };

		std::multimap<Key, DrawCall>::const_iterator kDrawLow { mDrawCalls.lower_bound(lowKey) };
		std::multimap<Key, DrawCall>::const_iterator kDrawUp { mDrawCalls.upper_bound(upKey) };

		if (kDrawLow != mDrawCalls.end())
			mDrawCalls.erase(kDrawLow, kDrawUp);

		std::map<Key, CommandBuffer>::iterator cmdLow { mCommands.lower_bound(lowKey) };
		std::map<Key, CommandBuffer>::iterator cmdUp { mCommands.upper_bound(upKey) };

		if (cmdLow != mCommands.end())
		{
			for (std::map<Key, CommandBuffer>::iterator it { cmdLow } ; it != cmdUp ; ++it)
				it->second.mIsDirty = true;

			mIsDirty = true;
		}

		sRenderPasses.erase(renderPassIdx);
	}

	std::vector<VkCommandBuffer> FrameGraph::commandBuffers(const RenderPass& kRenderPass, uint32_t subpassIndex) const
	{
		uint32_t renderPassIdx { renderPassIndex(kRenderPass, false) };
		if (renderPassIdx == Key::skMaxRenderPassIndex)
		{
			LOG(LogRHIWarning, "failed to find command buffers: unknown render pass")
			return {};
		}

		std::map<Key, CommandBuffer>::const_iterator kLow { mCommands.lower_bound({ 0u, 0u, false, subpassIndex, renderPassIdx }) };
		std::map<Key, CommandBuffer>::const_iterator kUp { mCommands.upper_bound({ Key::skMaxMaterialIndex - 1u, Key::skMaxSortingOrder - 1u, true, subpassIndex, renderPassIdx }) };

		if (kLow == mCommands.end())
		{
			LOG(LogRHIWarning, "failed to find command buffers: render pass " << renderPassIdx << " with subpass " << subpassIndex << " not found")
			return {};
		}

		std::vector<VkCommandBuffer> commandBuffers {};
		for (std::map<Key, CommandBuffer>::const_iterator it { kLow } ; it != kUp ; ++it)
			commandBuffers.emplace_back(it->second.mBuffer);

		return commandBuffers;
	}

	void FrameGraph::createCommands(uint32_t newCount)
	{
		// Allocating multiple buffers at once
		std::vector<VkCommandBuffer> buffers {};
		buffers.resize(newCount, VK_NULL_HANDLE);

		const Renderer* kRenderer { Service::get<Renderer>() };

		VkCommandBufferAllocateInfo allocInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
		allocInfo.commandPool = kRenderer->commandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_SECONDARY;
		allocInfo.commandBufferCount = newCount;

		VkResult result { vkAllocateCommandBuffers(Service::get<Device>()->device(), &allocInfo, buffers.data()) };
		if (result != VK_SUCCESS)
			THROW("failed to allocate command buffers with error: " + std::to_string(result))

		// Assigning each buffer to our command buffers
		uint32_t crtBuffer { 0u };
		for (std::multimap<Key, DrawCall>::const_iterator it { mDrawCalls.begin() } ; it != mDrawCalls.end() ; ++it)
		{
			if (mCommands.count(it->first))
				continue;

			if (crtBuffer >= newCount)
				THROW("failed to order command buffers with error: array out of range")

			mCommands.emplace(it->first, buffers[crtBuffer]);
			++crtBuffer;
		}
	}

	void FrameGraph::replaceDrawCallKey(const Key& kNewKey)
	{
		// Search with material index & render pass index
		std::multimap<Key, DrawCall>::const_iterator kLow { mDrawCalls.lower_bound({ kNewKey.mMaterialIndex - 1u, 0u, false, 0u, kNewKey.mRenderPassIndex }) };
		std::multimap<Key, DrawCall>::const_iterator kUp { mDrawCalls.upper_bound({ kNewKey.mMaterialIndex, Key::skMaxSortingOrder - 1u, true, Key::skMaxSubpassIndex - 1u,
																				  kNewKey.mRenderPassIndex }) };

		while (kLow != mDrawCalls.end() && kLow != kUp && kLow->first.mMaterialIndex != kNewKey.mMaterialIndex)
			++kLow;

		// Old key not found
		if (kLow == mDrawCalls.end() || kLow == kUp)
			return;

		// For all old draw calls
		Key oldKey { kLow->first };

		if (oldKey == kNewKey)
			return;

		std::multimap<Key, DrawCall>::node_type node {};
		while (!(node = mDrawCalls.extract(oldKey)).empty())
		{
			node.key() = kNewKey;
			mDrawCalls.insert(std::move(node));
		}
	}

	void FrameGraph::replaceCommandBufferKey(const Key& kNewKey)
	{
		// Search with material index & render pass index
		std::map<Key, CommandBuffer>::const_iterator kLow { mCommands.lower_bound({ kNewKey.mMaterialIndex - 1u, 0u, false, 0u, kNewKey.mRenderPassIndex }) };
		std::map<Key, CommandBuffer>::const_iterator kUp { mCommands.upper_bound({ kNewKey.mMaterialIndex, Key::skMaxSortingOrder - 1u, true, Key::skMaxSubpassIndex - 1u,
																				 kNewKey.mRenderPassIndex }) };

		while (kLow != mCommands.end() && kLow != kUp && kLow->first.mMaterialIndex != kNewKey.mMaterialIndex)
			++kLow;

		// Old key not found
		if (kLow == mCommands.end() || kLow == kUp)
			return;

		// Replace the old command buffer
		std::map<Key, CommandBuffer>::node_type node { mCommands.extract(kLow->first) };
		node.key() = kNewKey;
		mCommands.insert(std::move(node));

		// Dirty
		if (mCommands.count(kNewKey) != 0u)
			mCommands.at(kNewKey).mIsDirty = true;

		mIsDirty = true;
	}
}