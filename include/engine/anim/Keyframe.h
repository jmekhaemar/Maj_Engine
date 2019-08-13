#ifndef _KEYFRAME_H_
#define _KEYFRAME_H_

#include <vector>

#include <Transform.h>

namespace maj::anim
{ 
	class Keyframe final
	{
	public:
		Keyframe() = default;
		~Keyframe() = default;

		const math::Transf& transform(uint32_t boneIndex) const;
		const std::vector<math::Transf>& transforms() const;

		void addTransform(math::Transf&& mat);

	private:
		std::vector<math::Transf> mTransforms;
	};
}
#endif