#ifndef _ICON_H_
#define _ICON_H_

#include <vector>
#include <GLFW/glfw3.h>

#include <NonCopyable.h>

namespace maj::ww
{
	class Icon final : public NonCopyable
	{
	public:
		Icon(std::vector<const char*>&& names);
		~Icon();

		int32_t imageCount() const;
		const GLFWimage* images() const;

	private:
		std::vector<GLFWimage> mImages;
	};
}

#endif