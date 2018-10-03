#include "gui/CWidget.h"

#include <Window.h>

#include "scene/SceneHandler.h"

namespace maj::gui
{
	DEFINE_HASH_OVERLOAD(CWidget)

	CWidget::CWidget(JsonNode* root)
		: IComponent {}, mSpace { ECoordSpace::SCREEN }, mElements {}, mBuffer { nullptr }, mDrawEventIndex { 0u }, mCameraEventIndex { 0u }, mResizeEventIndex { 0u }
	{
		load(root);
		initialize();
	}

	CWidget::CWidget(scene::Actor* actor, ECoordSpace space)
		: IComponent { actor }, mSpace { space }, mElements {}, mBuffer { nullptr }, mDrawEventIndex { 0u }, mCameraEventIndex { 0u }, mResizeEventIndex { 0u }
	{
		initialize();
		onAttach(nullptr);
	}

	CWidget::~CWidget()
	{
		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		activeScene->mOnCameraChange -= mCameraEventIndex;

		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(0u) };
		viewport.mOnResized -= mResizeEventIndex;

		rhi::Buffer::release(mBuffer);

		removeElements();
	}

	void CWidget::initialize()
	{
		// Enabled/Disable
		mOnEnabled += [&]() -> void
		{
			for (IElement* element : mElements)
				element->enable(true);

			mDrawEventIndex = mActor->mOnDraw + std::bind(&CWidget::draw, this);
		};

		mOnDisabled += [&](bool destroyed) -> void
		{
			(void)destroyed;

			for (IElement* element : mElements)
				element->enable(false);

			if (mActor)
				mActor->mOnDraw -= mDrawEventIndex;
		};

		// World space ui
		mBuffer = rhi::Buffer::request(rhi::Buffer::EType::UNIFORM, sizeof(math::Mat4f));

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		mCameraEventIndex = activeScene->mOnCameraChange + [this] (const rhi::IDescriptor* kBuffer)->void
		{
			if (mSpace != ECoordSpace::WORLD)
				return;

			for (IElement* element : mElements)
				element->camera(kBuffer);
		};

		// Reparent all widget
		for (IElement* element : mElements)
			element->widget(this);

		// Handle screen reference
		ww::Viewport& viewport { Service::get<ww::Window>()->viewport(0u) };
		reference(viewport.extent());

		mResizeEventIndex = viewport.mOnResized + [this] (uint32_t id, const math::Vec2ui& kExtent) -> void
		{
			(void) id;
			if (mSpace == ECoordSpace::SCREEN)
				reference(kExtent);
		};
	}

	ECoordSpace CWidget::space() const
	{
		return mSpace;
	}

	void CWidget::space(ECoordSpace space)
	{
#ifndef MAJ_EDITOR
		if (mSpace == space)
			return;
#endif

		mSpace = space;
		mOnSpaceChanged(static_cast<ECoordSpace&&>(mSpace));

		if (mSpace == ECoordSpace::WORLD)
		{
			scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
			if (!activeScene)
				return;

			const scene::CCamera* kCamera { activeScene->cameras().main() };
			if (!kCamera)
				return;

			for (IElement* element : mElements)
				element->camera(kCamera->buffer());
		}
	}

	const math::Vec2ui& CWidget::reference() const
	{
		return mRefScreen;
	}

	void CWidget::reference(const math::Vec2ui& kRef)
	{
		math::Vec2f factor { mRefScreen.mX / static_cast<float>(kRef.mX), mRefScreen.mY / static_cast<float>(kRef.mY) };
		mRefScreen = kRef;

		for (IElement* element : mElements)
			element->resize(factor);
	}

	void CWidget::removeElements()
	{
		const std::vector<IElement*> kElements { mElements };
		mElements.clear();

		for (std::vector<IElement*>::const_iterator it { kElements.begin() } ; it != kElements.end() ; ++it)
			delete *it;
	}

	void CWidget::removeElement(uint32_t index)
	{
		uint8_t elemenCount { static_cast<uint8_t>(mElements.size()) };
		if (index >= elemenCount)
		{
			LOG(LogWarning, "failed to remove gui element: index out of range")
			return;
		}

		IElement* element { mElements[index] };
		mElements.erase(mElements.begin() + index);

		if (element)
			delete element;
	}

	void CWidget::removeElement(const IElement* kElement)
	{
		if (!kElement)
			return;

		for (std::vector<IElement*>::iterator it { mElements.begin() } ; it != mElements.end() ; ++it)
		{
			if (kElement != *it)
				continue;

			mElements.erase(it);
			delete kElement;
			return;
		}

		LOG(LogWarning, "failed to remove gui element: gui element in actor \'" << mActor->name() << "\' not found")
	}

	const rhi::Buffer::SubBuffer* CWidget::buffer() const
	{
		return mBuffer;
	}

	void CWidget::awake()
	{
		if (mSpace == ECoordSpace::SCREEN)
			return;

		scene::Scene* activeScene { Service::get<scene::SceneHandler>()->activeScene() };
		
		const rhi::IDescriptor* kBuffer { activeScene->cameras().main()->buffer() };
		for (IElement* element : mElements)
			element->camera(kBuffer);
	}

	void CWidget::onAttach(scene::Actor* previousActor)
	{
		if (previousActor)
			previousActor->mOnDraw -= mDrawEventIndex;

		if (mIsEnabled)
			mDrawEventIndex = mActor->mOnDraw + std::bind(&CWidget::draw, this);
	}

	void CWidget::draw() const
	{
		if (mSpace == ECoordSpace::WORLD)
		{
			const scene::CTransform* kActorTr { mActor->rootTransform() };
			if (!kActorTr)
				LOG(LogWarning, "failed to draw gui widget: actor \'" << mActor->name() << "\' has no root transform")
			else
			{
				// Inversing y-axis scale, because projection expects it upward but we use downward
				math::Mat4f mat { kActorTr->global().matrix() };
				mat.at(5u) *= -(static_cast<float>(mRefScreen.mY) / static_cast<float>(mRefScreen.mX));

				rhi::Buffer::MemoryMap memMap { *mBuffer };
				std::memcpy(memMap.mData, mat.elem(), sizeof(math::Mat4f));
			}
		}

		for (IElement* element : mElements)
			element->draw();
	}
}