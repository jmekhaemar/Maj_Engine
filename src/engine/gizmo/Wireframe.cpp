#include "gizmo/Wireframe.h"

#include <Renderer.h>
#include <Mat4.h>

#include "scene/SceneHandler.h"

namespace maj::gizmo
{
	const char* Wireframe::skMaterialName { "asset/default/wireframeMaterial" };
	const char* Wireframe::skLayoutName { "asset/default/wireframeLayout" };

	Wireframe::Wireframe(scene::IComponent* component, const resources::Model* kModel)
		: IGizmo { component }, mMaterialInstance { nullptr }, mkModel { kModel }, mBuffer { nullptr }, mCameraEventIndex { 0u }, mTransform {},
		mColor { 1.f , 1.f, 1.f }
	{
		if (!mkModel)
			THROW("failed to create wireframe: no model")

		if (!Asset<rhi::Material>::exists(skMaterialName))
			createMaterial();

		mMaterialInstance = new rhi::MaterialInstance { Asset<rhi::Material>::get(skMaterialName) };
		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Mat4f) + sizeof(math::Vec3Holder<float>));
		updateMaterial();
	}

	Wireframe::~Wireframe()
	{
		if (mMaterialInstance)
			delete mMaterialInstance;

		rhi::Buffer::release(mBuffer);

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		activeScene->mOnCameraChange -= mCameraEventIndex;
	}

	void Wireframe::transform(const std::function<math::Transf()>& kTransform)
	{
		mTransform = kTransform;
	}

	const math::Vec3Holder<float>& Wireframe::color() const
	{
		return mColor;
	}

	void Wireframe::color(const math::Vec3Holder<float>& kColor)
	{
		mColor = kColor;
	}

	void Wireframe::enable(GizmoHandler& handler, bool enabled)
	{
		IGizmo::enable(handler, enabled);

		if (enabled)
			Service::get<rhi::Renderer>()->frameGraph().add(mMaterialInstance, mkModel->geometry());
		else
			Service::get<rhi::Renderer>()->frameGraph().remove(mMaterialInstance);
	}

	void Wireframe::draw() const
	{
		math::Transf transf {};
		if (!mTransform)
		{
			const scene::Actor* kActor { mComponent->actor() };
			const scene::CTransform* kRootTr { kActor->rootTransform() };
			if (!kRootTr)
			{
				LOG(LogWarning, "faile de to draw wireframe: actor \'" << kActor->name() << "\' has no root transform")
				return;
			}

			transf = kRootTr->global();
		}
		else
			transf = mTransform();

		rhi::Buffer::MemoryMap memMap { *mBuffer };
		std::memcpy(memMap.mData, transf.matrix().elem(), sizeof(math::Mat4f));
		std::memcpy(memMap.mData + sizeof(math::Mat4f), &mColor, sizeof(math::Vec3Holder<float>));
	}

	void Wireframe::updateMaterial()
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

	void Wireframe::createMaterial()
	{
		rhi::Material::Info info { Service::get<rhi::Renderer>()->swapChain(0u).renderPass(), 0u, rhi::FrameGraph::Key::skMaxSortingOrder - 2u };
		info.mPipelineState.mCullMode = rhi::ECullModeFlag::NONE;
		info.mPipelineState.mPolygonMode = rhi::EPolygonMode::LINE;
		info.mPipelineState.mBindings = resources::Vertex::bindings();

		info.mShaders.reserve(2u);
		info.mShaders.emplace_back(rhi::EShaderStageFlag::VERTEX, "data/shaders/compiled/defaultWireframe.vert.spv");
		info.mShaders.emplace_back(rhi::EShaderStageFlag::FRAGMENT, "data/shaders/compiled/defaultWireframe.frag.spv");

		if (!Asset<rhi::MaterialLayout>::exists(skLayoutName))
		{
			rhi::LayoutSet sceneSet { { { 0u, { rhi::EDescriptorType::UNIFORM_BUFFER, 1u, rhi::EShaderStageFlag::VERTEX } } } };
			rhi::LayoutSet modelSet { { { 0u, { rhi::EDescriptorType::UNIFORM_BUFFER, 1u, rhi::EShaderStageFlag::VERTEX | rhi::EShaderStageFlag::FRAGMENT } } } };
			
			Asset<rhi::MaterialLayout>::add(skLayoutName, std::map<uint32_t, rhi::LayoutSet>{ { 0u, sceneSet }, { 1u, modelSet } }, rhi::MaterialLayout::skMaxMaterialInstance);
		}
		
		Asset<rhi::Material>::add(skMaterialName, Asset<rhi::MaterialLayout>::get(skLayoutName), std::move(info));
	}
}