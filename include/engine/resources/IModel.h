#ifndef _IMODEL_H_
#define _IMODEL_H_

#include <ISerializable.h>
#include <DrawCall.h>

namespace maj::resources
{
	class IModel : public NonCopyable, public ISerializable
	{
	public:
		virtual ~IModel() = default;

		virtual rhi::DrawCall::Geometry geometry() const = 0;
	};

}

#endif