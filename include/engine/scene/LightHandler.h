#ifndef _LIGHT_HANDLER_H_
#define _LIGHT_HANDLER_H_

#include <vector>

#include <NonCopyable.h>
#include <Buffer.h>

#include "scene/ICLight.h"

#define MAX_LIGHT_COUNT 20u

namespace maj::scene
{
	class LightHandler final : public NonCopyable
	{
	public:
		LightHandler();
		~LightHandler();

		void draw() const;

		void add(const ICLight* kLight);
		void remove(const ICLight* kLight);

		const rhi::Buffer::SubBuffer* buffer() const;

	private:
		bool find(const ICLight* kLight) const;

		rhi::Buffer::SubBuffer* mBuffer;
		std::vector<const ICLight*> mLights;
	};
}

#endif