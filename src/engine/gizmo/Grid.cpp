#include "gizmo/Grid.h"

#include <Renderer.h>

#include "scene/SceneHandler.h"

namespace maj::gizmo
{
	const char* Grid::skMaterialName { "asset/default/gridMaterial" };
	const char* Grid::skLayoutName { "asset/default/gridLayout" };

	Grid::Grid(scene::IComponent* component)
		: IGizmo { component }, mMaterialInstance { nullptr }, mCameraEventIndex { 0u }
	{
		if (!Asset<rhi::Material>::exists(skMaterialName))
			createMaterial();

		mMaterialInstance = new rhi::MaterialInstance { Asset<rhi::Material>::get(skMaterialName) };
		updateMaterial();
	}

	Grid::~Grid()
	{
		if (mMaterialInstance)
			delete mMaterialInstance;

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		activeScene->mOnCameraChange -= mCameraEventIndex;
	}

	void Grid::enable(GizmoHandler& handler, bool enabled)
	{
		// Don't add draw to the event, so don't call IGizmo::enable
		(void)handler;

		if (enabled)
			Service::get<rhi::Renderer>()->frameGraph().add(mMaterialInstance, std::vector<rhi::DrawCall::VertexInput>{ { 1u, 1u }});
		else
			Service::get<rhi::Renderer>()->frameGraph().remove(mMaterialInstance);
	}

	void Grid::draw() const
	{}

	void Grid::updateMaterial()
	{
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

	void Grid::createMaterial()
	{
		maj::rhi::Material::Info info { Service::get<rhi::Renderer>()->swapChain(0u).renderPass(), 0u, rhi::FrameGraph::Key::skMaxSortingOrder - 2u };
		info.mPipelineState.mTopology = maj::rhi::ETopology::POINT;

		info.mShaders.reserve(3u);
		info.mShaders.emplace_back(maj::rhi::EShaderStageFlag::VERTEX, "data/shaders/compiled/grid.vert.spv");
		info.mShaders.emplace_back(maj::rhi::EShaderStageFlag::GEOMETRY, "data/shaders/compiled/grid.geom.spv");
		info.mShaders.emplace_back(maj::rhi::EShaderStageFlag::FRAGMENT, "data/shaders/compiled/grid.frag.spv");

		if (!Asset<rhi::MaterialLayout>::exists(skLayoutName))
		{
			rhi::LayoutSet gridSet { { { 0u, { rhi::EDescriptorType::UNIFORM_BUFFER, 1u, rhi::EShaderStageFlag::GEOMETRY } } } };
			
			Asset<rhi::MaterialLayout>::add(skLayoutName, std::map<uint32_t, rhi::LayoutSet>{ { 0u, gridSet } }, rhi::MaterialLayout::skMaxMaterialInstance);
		}

		Asset<rhi::Material>::add(skMaterialName, Asset<rhi::MaterialLayout>::get(skLayoutName), std::move(info));
	}
}