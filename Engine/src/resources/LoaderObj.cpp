#include "resources/LoaderObj.h"

namespace maj::resources
{
	LoaderObj::LoaderObj()
		: Service { typeid(LoaderObj).name() }
	{}

	void LoaderObj::load(const char* kPath, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, Vertex::EComponentFlag availableData) const
	{
		std::ifstream stream{ kPath, std::fstream::in };
		if (!stream.is_open())
			THROW("can't open file")

		std::vector<math::Vec3Holder<float>> tempPos;
		std::vector<math::Vec2Holder<float>> tempUvs;
		std::vector<math::Vec3Holder<float>> tempNormals;
		std::vector<math::Vec3ui> tempIndices;

		parseObj(stream, tempPos, tempUvs, tempNormals, tempIndices, availableData);

		if (tempUvs.empty())
			tempUvs.emplace_back();

		if (tempNormals.empty())
			tempNormals.emplace_back();

		vertices.reserve(tempPos.size() * 2u);
		indices.reserve(tempIndices.size());

		std::unordered_map<Vertex, uint32_t> uniqueVertices {};
		uint32_t size { static_cast<uint32_t>(tempIndices.size()) };

		for (uint32_t i { 0u } ; i < size ; ++i)
		{
			Vertex vertex { tempPos[tempIndices[i].mX], tempUvs[tempIndices[i].mY], tempNormals[tempIndices[i].mZ] };
			if (uniqueVertices.count(vertex) == 0u)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.emplace_back(vertex);
			}
			indices.emplace_back(uniqueVertices[vertex]);
		}

		if ((availableData & Vertex::EComponentFlag::UV_NORMAL) == Vertex::EComponentFlag::UV_NORMAL)
			generateTangents(vertices, indices);
	}

	void LoaderObj::parseObj(std::ifstream& stream, std::vector<math::Vec3Holder<float>>& pos, std::vector<math::Vec2Holder<float>>& uvs, std::vector<math::Vec3Holder<float>>& normals,
							 std::vector<maj::math::Vec3ui>& indices, Vertex::EComponentFlag kAvailableData) const
	{
		std::string sTemp;
		while (!stream.eof())
		{
			switch (stream.get())
			{
				case '\n':
				continue;
				case 'v':
				{
					switch (stream.get())
					{
					case 't':
					{
						stream.get();
						math::Vec2Holder<float> temp;
						for (uint8_t i{ 0u }; i < 2u; ++i)
						{
							std::getline(stream, sTemp, i == 1 ? '\n' : ' ');
							if (i == 1)
								temp[i] = 1.f - std::stof(sTemp);
							else
								temp[i] = std::stof(sTemp);
						}
						uvs.emplace_back(temp);
						break;
					}
					case 'n':
					{
						stream.get();
						math::Vec3Holder<float> temp;
						for (uint8_t i{ 0u }; i < 3u; ++i)
						{
							std::getline(stream, sTemp, i == 2u ? '\n' : ' ');
							temp[i] = std::stof(sTemp);
						}
						normals.emplace_back(temp);
						break;
					}
					default:
					{
						math::Vec3Holder<float> temp;
						for (uint8_t i{ 0u }; i < 3u; ++i)
						{
							std::getline(stream, sTemp, i == 2u ? '\n' : ' ');
							temp[i] = std::stof(sTemp);
						}
						pos.emplace_back(temp);
						break;
					}
					}
					continue;
				}
				case 'f':
				{
					stream.get();
					parseFace(stream, indices, kAvailableData);
					continue;
				}
				default:
				{
					std::getline(stream, sTemp);
					continue;
				}
			}
		}
	}

	void LoaderObj::parseFace(std::ifstream& stream, std::vector<math::Vec3ui>& allIndices, Vertex::EComponentFlag availableData) const
	{
		std::string sTemp {};
		for (uint8_t i{ 0u }; i < 3u; ++i)
		{
			switch (availableData)
			{
				case Vertex::EComponentFlag::POSITION:
				{
					std::getline(stream, sTemp, i == 2u ? '\n' : ' ');
					allIndices.emplace_back(std::stoi(sTemp) - 1u, 0u, 0u);
					break;
				}
				case (Vertex::EComponentFlag::POSITION | Vertex::EComponentFlag::UV):
				{
					math::Vec2ui temp;
					for (uint8_t j { 0u }; j < 2u; ++j)
					{
						std::getline(stream, sTemp, j == 1u ? i == 2u ? '\n' : ' ' : '/');
						temp[j] = std::stoi(sTemp) - 1u;
					}
					allIndices.emplace_back(temp.mX, temp.mY, 0u);
					break;
				}
				case (Vertex::EComponentFlag::POSITION | Vertex::EComponentFlag::NORMAL):
				{
					math::Vec2ui temp;
					for (uint8_t j { 0u }; j < 2u; ++j)
					{
						std::getline(stream, sTemp, j == 1u ? i == 2u ? '\n' : ' ' : '/');
						temp[j] = std::stoi(sTemp) - 1u;
					}
					allIndices.emplace_back(temp.mX, 0u, temp.mY);
					break;
				}
				case (Vertex::EComponentFlag::POSITION | Vertex::EComponentFlag::UV | Vertex::EComponentFlag::NORMAL):
				{
					math::Vec3ui temp;
					for (uint8_t j { 0u }; j < 3u; ++j)
					{
						std::getline(stream, sTemp, j == 2u ? i == 2u ? '\n' : ' ' : '/');
						temp[j] = std::stoi(sTemp) - 1u;
					}
					allIndices.emplace_back(temp.mX, temp.mY, temp.mZ);
					break;
				}

				default:
				THROW("data format unsupported")
			}
		}
	}

	void LoaderObj::generateTangents(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) const
	{
		uint32_t triangleCount { static_cast<uint32_t>(indices.size()) / 3u };
		for (uint32_t i { 0u } ; i < triangleCount ; ++i)
		{
			Vertex& p0 { vertices[indices[i * 3u]] };
			Vertex& p1 { vertices[indices[i * 3u + 1u]] };
			Vertex& p2 { vertices[indices[i * 3u + 2u]] };

			addTangent(p0, p1, p2);
		}

		uint32_t vertexCount { static_cast<uint32_t>(vertices.size()) };
		for (uint32_t i { 0u } ; i < vertexCount ; ++i)
		{
			math::Vec3f n { vertices[i].normal() };
			math::Vec3f t { vertices[i].tangent() };

			t -= n * n.dot(t);
			t.normalize();

			vertices[i].tangent(t.data());
		}
	}

	void LoaderObj::addTangent(Vertex& p0, Vertex& p1, Vertex& p2) const
	{
		math::Vec3f pos0 { p0.position() }, pos1 { p1.position() }, pos2 { p2.position() };
		math::Vec2f uv0 { p0.uv() }, uv1 { p1.uv() }, uv2 { p2.uv() };

		math::Vec3f q1 { pos1 - pos0 }, q2 { pos2 - pos0 };
		math::Vec2f w1 { uv1 - uv0 }, w2 { uv2 - uv0 };

		float r { w1.mX * w2.mY - w2.mX * w1.mY };
		if (math::Helper<float>::areSame(r, 0.f))
			return;

		math::Vec3f s {	w2.mY * q1.mX - w1.mY * q2.mX,
						w2.mY * q1.mY - w1.mY * q2.mY,
						w2.mY * q1.mZ - w1.mY * q2.mZ };
		s *= 1.f / r;
		
		math::Vec3f t0 { p0.tangent() }, t1 { p1.tangent() }, t2 { p2.tangent() };
		t0 += s;
		t1 += s;
		t2 += s;

		p0.tangent(t0.data());
		p1.tangent(t1.data());
		p2.tangent(t2.data());
	}
}