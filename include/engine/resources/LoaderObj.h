#ifndef _LOADER_H_
#define _LOADER_H_

#include <vector>

#include <Service.h>

#include "resources/Vertex.h"

namespace maj::resources
{
	class LoaderObj final : public Service
	{
	public:
		LoaderObj();
		~LoaderObj() = default;

		void load(const char* kPath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Vertex::EComponentFlag availableData) const;
	
	private :
		void parseObj(std::ifstream& stream, std::vector<math::Vec3Holder<float>>& pos, std::vector<math::Vec2Holder<float>>& uvs, std::vector<math::Vec3Holder<float>>& normals,
					  std::vector<maj::math::Vec3ui>& indices, Vertex::EComponentFlag kAvailableData) const;
		void parseFace(std::ifstream& stream, std::vector<math::Vec3ui>& allIndices, Vertex::EComponentFlag kAvailableData) const;

		void generateTangents(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const;
		void addTangent(Vertex& p0, Vertex& p1, Vertex& p2) const;
	};
}

#endif