#include "gizmo/Handle.h"

#include <Renderer.h>

#include "scene/SceneHandler.h"

namespace maj::gizmo
{
	const char* Handle::skMaterialName { "asset/default/handleMaterial" };
	const char* Handle::skLayoutName { "asset/default/handleLayout" };

	Handle::Handle(scene::IComponent* transform)
		: IGizmo { transform }, mMaterialInstance { nullptr }, mBuffer { nullptr }, mCameraEventIndex { 0u }
	{
		if (!dynamic_cast<scene::CTransform*>(mComponent))
			THROW("failed to create handle: component must be a CTransform")

		if (!Asset<rhi::Material>::exists(skMaterialName))
			createMaterial();

		mMaterialInstance = new rhi::MaterialInstance { Asset<rhi::Material>::get(skMaterialName) };
		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Mat4f));
		updateMaterial();
	}

	Handle::~Handle()
	{
		if (mMaterialInstance)
			delete mMaterialInstance;

		rhi::Buffer::release(mBuffer);

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		activeScene->mOnCameraChange -= mCameraEventIndex;
	}

	void Handle::enable(GizmoHandler& handler, bool enabled)
	{
		IGizmo::enable(handler, enabled);

		if (enabled)
			Service::get<rhi::Renderer>()->frameGraph().add(mMaterialInstance, std::vector<rhi::DrawCall::VertexInput>{ { 1u, 1u }});
		else
			Service::get<rhi::Renderer>()->frameGraph().remove(mMaterialInstance);
	}

	void Handle::draw() const
	{
		scene::CTransform* transform { static_cast<scene::CTransform*>(mComponent) };
		math::Transf transf { transform->global() };
		transf.scale({ 1.f }); // Reset scale

		rhi::Buffer::MemoryMap memMap { *mBuffer };
		std::memcpy(memMap.mData, transf.matrix().elem(), sizeof(math::Mat4f));
	}

	void Handle::updateMaterial()
	{
		mMaterialInstance->descriptor(1u, 0u, mBuffer);
		mMaterialInstance->update(1u, 0u);

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		mCameraEventIndex = activeScene->mOnCameraChange + [this] (const rhi::IDescriptor* kBuffer) -> void
		{
			mMaterialInstance->descriptor(0u, 0u, kBuffer);
			mMaterialInstance->update(0u, 0u);
		};

		scene::CCamera* mainCamera { activeScene->cameras().main() };
		if (mainCamera)
		{
			mMaterialInstance->descriptor(0u, 0u, mainCamera->buffer());
			mMaterialInstance->update(0u, 0u);
		}
	}

	void Handle::createMaterial()
	{
		maj::rhi::Material::Info info { Service::get<rhi::Renderer>()->swapChain(0u).renderPass(), 0u, rhi::FrameGraph::Key::skMaxSortingOrder - 2u };
		info.mPipelineState.mTopology = maj::rhi::ETopology::POINT;

		info.mShaders.reserve(3u);
		info.mShaders.emplace_back(maj::rhi::EShaderStageFlag::VERTEX, "data/shaders/compiled/handle.vert.spv");
		info.mShaders.emplace_back(maj::rhi::EShaderStageFlag::GEOMETRY, "data/shaders/compiled/handle.geom.spv");
		info.mShaders.emplace_back(maj::rhi::EShaderStageFlag::FRAGMENT, "data/shaders/compiled/handle.frag.spv");

		if (!Asset<rhi::MaterialLayout>::exists(skLayoutName))
		{
			rhi::LayoutSet cameraSet { { { 0u, { rhi::EDescriptorType::UNIFORM_BUFFER, 1u, rhi::EShaderStageFlag::GEOMETRY } } } };
			rhi::LayoutSet modelSet { { { 0u, { rhi::EDescriptorType::UNIFORM_BUFFER, 1u, rhi::EShaderStageFlag::GEOMETRY } } } };

			Asset<rhi::MaterialLayout>::add(skLayoutName, std::map<uint32_t, rhi::LayoutSet>{ { 0u, cameraSet }, { 1u, modelSet } }, rhi::MaterialLayout::skMaxMaterialInstance);
		}

		Asset<rhi::Material>::add(skMaterialName, Asset<rhi::MaterialLayout>::get(skLayoutName), std::move(info));
	}
}