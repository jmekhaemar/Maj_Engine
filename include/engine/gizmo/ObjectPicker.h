#ifndef _OBJECT_PICKER_H_
#define _OBJECT_PICKER_H_

#include <Framebuffer.h>

namespace maj::gizmo
{
	class ObjectPicker : public NonCopyable
	{
	private:
		static rhi::RenderPass::Info renderPassInfo();

	public:
		ObjectPicker();
		~ObjectPicker();

		uint32_t pick(const math::Vec2ui& kPosition);

		const rhi::RenderPass& renderPass() const;

	private:
		void createImages(const math::Vec2ui& kExtent);
		void createFramebuffer(const math::Vec2ui& kExtent);

		void resize(const math::Vec2ui& kExtent);

		rhi::RenderPass mRenderPass;
		rhi::Fence mSubmitFinished;
		std::vector<rhi::Image> mImages;
		std::unique_ptr<rhi::Framebuffer> mFramebuffer;
		std::unique_ptr<rhi::Image> mPicker;

		uint32_t mRecordEvent;
		uint32_t mResizeEvent;
		uint32_t mDrawEvent;
	};
}

#endif