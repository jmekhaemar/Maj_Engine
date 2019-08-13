#include "gizmo/Sprite.h"

#include <Renderer.h>
#include <Window.h>

#include "scene/SceneHandler.h"
#include "scene/CTransform.h"

namespace maj::gizmo
{
	const char* Sprite::skMaterialName { "asset/default/spriteMaterial" };
	const char* Sprite::skLayoutName { "asset/default/spriteLayout" };

	Sprite::Sprite(scene::IComponent* component, const rhi::Texture* kTexture)
		: IGizmo { component }, mMaterialInstance { nullptr }, mkTexture { kTexture }, mBuffer { nullptr }, mColor {}, mSize { 0.f, 0.f }, mCameraEventIndex { 0u },
		mResizeIndex { 0u }, mSelectionId { 0u }
	{
		if (!mkTexture)
			THROW("failed to create gizmo sprite: no texture")

		if (!Asset<rhi::Material>::exists(skMaterialName))
			createMaterial();

		mMaterialInstance = new rhi::MaterialInstance { Asset<rhi::Material>::get(skMaterialName) };
		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, 2u * sizeof(math::Vec4Holder<float>));
		updateMaterial();

		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(0u) };
		resize(viewport.extent());
		mResizeIndex = viewport.mOnResized + [this] (uint8_t id, const math::Vec2ui& kExtent) -> void
		{
			(void) id;
			resize(kExtent);
		};
	}

	Sprite::~Sprite()
	{
		if (mMaterialInstance)
			delete mMaterialInstance;

		rhi::Buffer::release(mBuffer);

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		activeScene->mOnCameraChange -= mCameraEventIndex;

		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(0u) };
		viewport.mOnResized -= mResizeIndex;
	}

	void Sprite::color(const std::function<math::Vec3Holder<float>()>& kColor)
	{
		mColor = kColor;
	}

	void Sprite::enable(GizmoHandler& handler, bool enabled)
	{
		IGizmo::enable(handler, enabled);

		if (enabled)
		{
			rhi::DrawCall::Geometry quad { { rhi::DrawCall::VertexInput { 1u, 4u } } };
			Service::get<rhi::Renderer>()->frameGraph().add(mMaterialInstance, quad);
			mSelectionId = Service::get<GizmoHandler>()->selection().add(mMaterialInstance, quad);

			Selector* selector { Service::get<GizmoHandler>()->selection().selector(mSelectionId) };
			if (selector)
			{
				selector->mOnSelected += [this](bool selected) -> void
				{
					Service::get<GizmoHandler>()->selectActor(selected ? mComponent->actor() : nullptr);
				};
			}
		}
		else
		{
			Service::get<rhi::Renderer>()->frameGraph().remove(mMaterialInstance);
			Service::get<GizmoHandler>()->selection().remove(mSelectionId);
		}
	}

	void Sprite::draw() const
	{
		const scene::Actor* kActor { mComponent->actor() };
		const scene::CTransform* kRootTr { kActor->rootTransform() };
		if (!kRootTr)
		{
			LOG(LogWarning, "failed to draw gizmo sprite: actor \'" << kActor->name() << "\' has no root transform")
			return;
		}

		math::Vec3Holder<float> pos { kRootTr->global().position().data() };
		math::Vec3Holder<float> color { 1.f, 1.f, 1.f };

		if (mColor)
			color = mColor();

		rhi::Buffer::MemoryMap memMap { *mBuffer };
		std::memcpy(memMap.mData, &pos, sizeof(math::Vec3Holder<float>));
		std::memcpy(memMap.mData + sizeof(math::Vec3Holder<float>), &mSize.mX, sizeof(float));
		std::memcpy(memMap.mData + sizeof(math::Vec4Holder<float>), &color, sizeof(math::Vec3Holder<float>));
		std::memcpy(memMap.mData + sizeof(math::Vec3Holder<float>) + sizeof(math::Vec4Holder<float>), &mSize.mY, sizeof(float));
	}

	void Sprite::updateMaterial()
	{
		mMaterialInstance->descriptor(1u, 0u, mBuffer);
		mMaterialInstance->descriptor(1u, 1u, mkTexture);
		mMaterialInstance->update(1u);

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		mCameraEventIndex = activeScene->mOnCameraChange + [this] (const rhi::IDescriptor* kBuffer) -> void
		{
			mMaterialInstance->descriptor(0u, 0u, kBuffer);
			mMaterialInstance->update(0u, 0u);

			Selector* selector { Service::get<GizmoHandler>()->selection().selector(mSelectionId) };
			if (selector)
			{
				selector->materialInstance()->descriptor(0u, 0u, kBuffer);
				selector->materialInstance()->update(0u, 0u);
			}
		};

		scene::CCamera* mainCamera { activeScene->cameras().main() };
		if (mainCamera)
		{
			mMaterialInstance->descriptor(0u, 0u, mainCamera->buffer());
			mMaterialInstance->update(0u, 0u);
		}
	}

	void Sprite::createMaterial()
	{
		rhi::Material::Info info { Service::get<rhi::Renderer>()->swapChain(0u).renderPass(), 0u, rhi::FrameGraph::Key::skMaxSortingOrder - 1u };
		info.mPipelineState.mTopology = maj::rhi::ETopology::TRIANGLE_STRIP;
		info.mPipelineState.mCullMode = rhi::ECullModeFlag::NONE;
		info.mPipelineState.mDepthWriteEnabled = false;
		info.mPipelineState.mColorBlend.mBlendOp = maj::rhi::EBlendOp::ADD;
		info.mPipelineState.mColorBlend.mSrcFactor = maj::rhi::EBlendFactor::SRC_A;
		info.mPipelineState.mColorBlend.mDstFactor = maj::rhi::EBlendFactor::ONE_MINUS_SRC_A;

		info.mShaders.reserve(2u);
		info.mShaders.emplace_back(rhi::EShaderStageFlag::VERTEX, "data/shaders/compiled/defaultSpriteGizmo.vert.spv");
		info.mShaders.emplace_back(rhi::EShaderStageFlag::FRAGMENT, "data/shaders/compiled/defaultSpriteGizmo.frag.spv");

		if (!Asset<rhi::MaterialLayout>::exists(skLayoutName))
		{
			rhi::LayoutSet sceneSet { { { 0u, { rhi::EDescriptorType::UNIFORM_BUFFER, 1u, rhi::EShaderStageFlag::VERTEX } } } };
			rhi::LayoutSet modelSet { { { 0u, { rhi::EDescriptorType::UNIFORM_BUFFER, 1u, rhi::EShaderStageFlag::VERTEX | rhi::EShaderStageFlag::FRAGMENT } },
										{ 1u, { rhi::EDescriptorType::TEXTURE, 1u, rhi::EShaderStageFlag::FRAGMENT } } } };

			Asset<rhi::MaterialLayout>::add(skLayoutName, std::map<uint32_t, rhi::LayoutSet>{ { 0u, sceneSet }, { 1u, modelSet } }, rhi::MaterialLayout::skMaxMaterialInstance);
		}

		Asset<rhi::Material>::add(skMaterialName, Asset<rhi::MaterialLayout>::get(skLayoutName), std::move(info));
	}

	void Sprite::resize(const math::Vec2ui& kExtent)
	{
		const math::Vec3ui& kImageExtent { mkTexture->image()->info().mExtent };

		mSize.mX = static_cast<float>(kImageExtent.mX) / static_cast<float>(kExtent.mX);
		mSize.mY = static_cast<float>(kImageExtent.mY) / static_cast<float>(kExtent.mY);
	}
}