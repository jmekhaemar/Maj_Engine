#include "gizmo/Selector.h"

#include <algorithm>

#include <Renderer.h>

namespace maj::gizmo
{
	Selector::Selector(uint32_t id, const rhi::MaterialInstance* kMaterialInstance, const rhi::RenderPass& kRenderPass, const rhi::DrawCall::Geometry& kGeometry)
		: mLayout {}, mMaterial {}, mMaterialInstance {}, mGeometry { kGeometry }, mBuffer { nullptr }
	{
		try
		{
			if (!kMaterialInstance)
				THROW("no material instance")

			const rhi::Material& kMaterial { kMaterialInstance->material() };
			const rhi::MaterialLayout& kLayout { kMaterial.layout() };

			rhi::LayoutSet set { { { 0u, { rhi::EDescriptorType::UNIFORM_BUFFER, 1u, rhi::EShaderStageFlag::FRAGMENT } } } };
			std::map<uint32_t, rhi::LayoutSet> sets { { 3u, set } };

			mLayout = std::make_unique<rhi::MaterialLayout>(kLayout, std::move(sets));
			
			rhi::Material::Info info { kMaterial.info(), kRenderPass, 0u };
			// Find fragment shader and replace it
			for (std::vector<rhi::Shader>::const_iterator it { info.mShaders.begin() } ; it != info.mShaders.end() ; )
			{
				// If contains fragment then remove the fragment aspect
				rhi::EShaderStageFlag flag { it->stage() };
				if (flag != rhi::EShaderStageFlag::FRAGMENT)
				{
					flag &= ~(rhi::EShaderStageFlag::FRAGMENT);

					++it;
					continue;
				}

				// If only fragment then erase the shader
				it = info.mShaders.erase(it);
			}
			info.mShaders.emplace_back(maj::rhi::EShaderStageFlag::FRAGMENT, "data/shaders/compiled/selection.frag.spv");

			// Reset blend states
			info.mPipelineState.mDepthWriteEnabled = true;
			info.mPipelineState.mColorBlend = {};
			info.mPipelineState.mAlphaBlend = {};

			mMaterial = std::make_unique<rhi::Material>(kMaterial, std::move(info), mLayout.get());
			mMaterialInstance = std::make_unique<rhi::MaterialInstance>(*kMaterialInstance, mMaterial.get());
		}
		catch (const std::exception& e)
		{
			LOG(LogWarning, "failed to create selector: " << e.what())
		}

		if (mMaterialInstance)
		{
			mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Vec4Holder<float>));

			// Convert to color, [ 0 ; 255 ] to [ 0 ; 1 ]
			Id colorId { id };
			math::Vec4Holder<float> color { static_cast<float>(colorId.rgba[0u]) / 255.f, static_cast<float>(colorId.rgba[1u]) / 255.f, static_cast<float>(colorId.rgba[2u]) / 255.f,
											static_cast<float>(colorId.rgba[3u]) / 255.f };

			rhi::Buffer::MemoryMap memMap { *mBuffer };
			std::memcpy(memMap.mData, &color, sizeof(math::Vec4Holder<float>));

			mMaterialInstance->descriptor(3u, 0u, mBuffer);
			mMaterialInstance->update();
			Service::get<rhi::Renderer>()->frameGraph().add(mMaterialInstance.get(), mGeometry);
		}
	}

	Selector::~Selector()
	{
		rhi::Buffer::release(mBuffer);
	}

	const rhi::MaterialInstance* Selector::materialInstance() const
	{
		return mMaterialInstance.get();
	}

	rhi::MaterialInstance* Selector::materialInstance()
	{
		return mMaterialInstance.get();
	}
}