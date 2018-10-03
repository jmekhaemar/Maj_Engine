#include "resources/AssetsHandler.h"

#include <experimental/filesystem>
#include <forward_list>

#include <Logging.h>
#include <Renderer.h>
namespace maj::resources
{
	AssetsHandler::AssetsHandler()
		: Service { typeid(AssetsHandler).name() }
	{
#ifdef MAJ_EDITOR
		std::forward_list<std::pair<uint32_t, uint32_t>> ids {};
		for (const std::experimental::filesystem::directory_entry& entry : std::experimental::filesystem::recursive_directory_iterator("asset/default/"))
		{
			const std::experimental::filesystem::path& kPath { entry.path() };
			if (kPath.extension().generic_string() != ".asset")
				continue;

			std::pair<uint32_t, uint32_t> id { read(kPath) };
			if (id.first != 0u)
				ids.emplace_front(id);
		}

		for (const std::pair<uint32_t, uint32_t>& kId : ids)
		{
			switch (kId.first)
			{
				case meta::Class<gui::Font>::hash():
				Asset<gui::Font>::add(kId.second);
				break;

				case meta::Class<rhi::MaterialLayout>::hash():
				Asset<rhi::MaterialLayout>::add(kId.second);
				break;

				case meta::Class<rhi::Image>::hash():
				Asset<rhi::Image>::add(kId.second);
				break;

				case meta::Class<physic::PhysicMaterial>::hash():
				Asset<physic::PhysicMaterial>::add(kId.second);
				break;

				case meta::Class<rhi::Material>::hash():
				Asset<rhi::Material>::add(kId.second);
				break;

				case meta::Class<rhi::Texture>::hash():
				Asset<rhi::Texture>::add(kId.second);
				break;

				case meta::Class<Model>::hash():
				Asset<Model>::add(kId.second);
				break;

				case meta::Class<SkeletalModel>::hash():
				Asset<SkeletalModel>::add(kId.second);
				break;

				default:
				break;
			}
		}
#endif
	}

	AssetsHandler::~AssetsHandler()
	{
#ifdef MAJ_EDITOR
		bool save { true };
#else
		bool save { false };
#endif

		Asset<gui::Font>::clear(save);
		Asset<physic::PhysicMaterial>::clear(save);
		Asset<rhi::Material>::clear(save);
		Asset<rhi::MaterialLayout>::clear(save);
		Asset<rhi::Texture>::clear(save);
		Asset<rhi::Image>::clear(save);
		Asset<Model>::clear(save);
		Asset<SkeletalModel>::clear(save);
	}

	void AssetsHandler::load(const char* kRootPath)
	{
		for (const std::experimental::filesystem::directory_entry& entry : std::experimental::filesystem::recursive_directory_iterator(kRootPath))
		{
			const std::experimental::filesystem::path& kPath { entry.path() };
			if (kPath.extension().generic_string() != ".asset")
				continue;

			read(kPath);
		}
	}

	void AssetsHandler::clean()
	{
		Asset<gui::Font>::clean();
		Asset<physic::PhysicMaterial>::clean();
		Asset<rhi::Material>::clean();
		Asset<rhi::MaterialLayout>::clean();
		Asset<rhi::Texture>::clean();
		Asset<rhi::Image>::clean();
		Asset<Model>::clean();
		Asset<SkeletalModel>::clean();
	}
	
	std::pair<uint32_t, uint32_t> AssetsHandler::read(const std::experimental::filesystem::path& kPath)
	{
		LOG(LogInfo, "reading asset \'" << kPath << "\'")
		std::string name { kPath.generic_string() };
		std::unique_ptr<JsonNode> root {};

		try
		{
			 root = JsonNode::parse(name.c_str());
		}
		catch (const std::exception& e)
		{
			LOG(LogWarning, "failed to read asset: failed to load asset \'" << name << "\': " << e.what())
			return { 0u, 0u };
		}

		uint32_t type { (*root)["type"]->getUint32() };
		uint32_t guid { (*root)["mGuid"]->getUint32() };

		switch (type)
		{
			case meta::Class<gui::Font>::hash():
			Asset<gui::Font>::initialize(guid, name);
			break;

			case meta::Class<rhi::MaterialLayout>::hash():
			Asset<rhi::MaterialLayout>::initialize(guid, name);
			break;

			case meta::Class<rhi::Image>::hash():
			Asset<rhi::Image>::initialize(guid, name);
			break;

			case meta::Class<physic::PhysicMaterial>::hash():
			Asset<physic::PhysicMaterial>::initialize(guid, name);
			break;

			case meta::Class<rhi::Material>::hash():
			Asset<rhi::Material>::initialize(guid, name);
			break;

			case meta::Class<rhi::Texture>::hash():
			Asset<rhi::Texture>::initialize(guid, name);
			break;

			case meta::Class<Model>::hash():
			Asset<Model>::initialize(guid, name);
			break;

			case meta::Class<SkeletalModel>::hash():
			Asset<SkeletalModel>::initialize(guid, name);
			break;

			default:
			LOG(LogWarning, "failed to read asset: type (" << type << ") of asset \'" << name << "\' not handled")
			return { 0u, guid };
		}

		return { type, guid };
	}
}