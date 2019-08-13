#include "scene/SceneHandler.h"

#include "resources/AssetsHandler.h"
#include <Renderer.h>

namespace maj::scene
{
	DEFINE_HASH_OVERLOAD(SceneHandler::SceneConfig)

	void SceneHandler::SceneConfig::initialize()
	{}

	std::string SceneHandler::sceneFromPath(const std::experimental::filesystem::path& kPath)
	{
		const std::string kStem { kPath.stem().generic_string() };
		if (!kPath.has_parent_path())
			return kStem;

		const std::experimental::filesystem::path kParentPath { kPath.parent_path() };
		const std::string kParentStr { kParentPath.generic_string() };

		if (kParentStr == ".")
			return kStem;

		return sceneFromPath(kParentPath) + "/" + kStem;
	}

	SceneHandler::SceneHandler()
		: Service { typeid(SceneHandler).name() }, mActiveScene { nullptr }
	{}

	SceneHandler::~SceneHandler()
	{
		mConfig.save("Scene.config");
	}

	void SceneHandler::load(const char* kRootPath)
	{
		std::unique_ptr<JsonNode> root { JsonNode::parse((std::string { kRootPath } + "/Scene.config").c_str()) };
		mConfig.load(root.get());

		for (std::unordered_map<uint32_t, std::string>::iterator ite{ mConfig.mScenesOrder.begin()}; ite != mConfig.mScenesOrder.end(); ite++)
		{
			const std::experimental::filesystem::path& kPath {ite->second + ".scene" };
			const std::string kSceneName { sceneFromPath(kPath) };

			mScenes.try_emplace(kSceneName, kSceneName, ite->first);
		}
	}

	const Scene* SceneHandler::activeScene() const
	{
		return mActiveScene;
	}

	Scene* SceneHandler::activeScene()
	{
		return mActiveScene;
	}

	void SceneHandler::activeScene(const std::experimental::filesystem::path& kPath)
	{
		std::string sceneName { sceneFromPath(kPath) };
		if (mScenes.count(sceneName) == 0)
		{
			LOG(LogWarning, "failed to set active scene: scene \'" << sceneName << "\' not found");
			return;
		}
		
		//mFunc = [this, kName]() -> void
		if(mActiveScene)
		{
			if(mActiveScene->name() == sceneName)
			{ 
				LOG(LogWarning, "failed to set active scene: already in scene \'" << sceneName << "\'");
				return;
			}

			mActiveScene->removeActors();
			mActiveScene = nullptr;

			Service::get<rhi::Renderer>()->frameGraph().clear();
		}

		mActiveScene = &mScenes.at(sceneName);

		try
		{
			std::unique_ptr<JsonNode> root { JsonNode::parse((sceneName + ".scene").c_str()) };
			mActiveScene->load(root.get());
			mActiveScene->initialize();

			Service::get<resources::AssetsHandler>()->clean();
		}
		catch (const std::exception& e)
		{
#ifdef MAJ_EDITOR
			LOG(LogWarning, "failed to set active scene: failed to load scene \'" << sceneName << "\': " << e.what())
#else
			THROW("failed to set active scene: failed to load scene \'" + sceneName + "\': " +std::string { e.what() })
#endif
		}
	}

	void SceneHandler::activeScene(uint16_t order)
	{
		if (mConfig.mScenesOrder.count(order) == 0u)
		{
			LOG(LogWarning, "failed to set active scene: scene of order " << order << " not found");
			return;
		}

		//mFunc = [this, order]() -> void
		if (mActiveScene)
		{
			if (mActiveScene->order() == order)
			{
				LOG(LogWarning, "failed to set active scene: already in scene of order " << order);
				return;
			}

			mActiveScene->removeActors();
			mActiveScene = nullptr;

			Service::get<rhi::Renderer>()->frameGraph().clear();
		}

		std::experimental::filesystem::path path { mConfig.mScenesOrder.at(order) };
		std::string sceneName { sceneFromPath(path) };
		mActiveScene = &mScenes.at(sceneName);

		try
		{
			std::unique_ptr<JsonNode> root { JsonNode::parse((sceneName + ".scene").c_str()) };
			mActiveScene->load(root.get());
			mActiveScene->initialize();

			Service::get<resources::AssetsHandler>()->clean();
		}
		catch (const std::exception& e)
		{
#ifdef MAJ_EDITOR
			LOG(LogWarning, "failed to set active scene: failed to load scene of order " << order << ": " << e.what())
#else
			THROW("failed to set active scene: failed to load scene of order " + std::to_string(order) + ": " + std::string{ e.what() })
#endif
		}
	}

	const Scene* SceneHandler::scene(const char* kStr) const
	{
		if (mScenes.count(kStr) == 0u)
		{
			LOG(LogWarning, "failed to find scene: scene \'" << kStr << "\' not found")
			return nullptr;
		}

		return &mScenes.at(kStr);
	}

	Scene* SceneHandler::scene(const char* kStr)
	{
		if (mScenes.count(kStr) == 0u)
		{
			LOG(LogWarning, "failed to find scene: scene \'" << kStr << "\' not found")
			return nullptr;
		}

		return &mScenes.at(kStr);
	}

	void SceneHandler::rename(const std::experimental::filesystem::path& kOldPath, const std::experimental::filesystem::path& kNewPath)
	{
		std::string oldName { sceneFromPath(kOldPath) };
		std::string newName { sceneFromPath(kNewPath) };

		if (mScenes.count(oldName) == 0u || mScenes.count(newName) != 0u)
			return;

		std::unordered_map<std::string, Scene>::node_type node { mScenes.extract(oldName) };
		node.key() = newName;
		mScenes.insert(std::move(node));

		Scene& scene { mScenes.at(newName) };
		scene.name(newName.c_str());

		mConfig.mScenesOrder.at(scene.order()) = newName;

		std::unique_ptr<JsonNode> root{ JsonNode::parse((newName +".scene").c_str()) };
		(*root)["mName"]->setString(newName);
		root->save((newName + ".scene").c_str());
	}

	void SceneHandler::addScene(const std::experimental::filesystem::path& kPath)
	{
		std::string sceneName { sceneFromPath(kPath) };
		if (mScenes.count(sceneName))
		{
			LOG(LogWarning, "failed to add scene: scene \'" << sceneName << "\' already exists")
			return;
		}

		int32_t order{-1};
		for (std::unordered_map<uint32_t, std::string>::iterator ite{ mConfig.mScenesOrder.begin() }; ite != mConfig.mScenesOrder.end(); ite++)
		{ 
			if(order < static_cast<int32_t>(ite->first))
				order = static_cast<int32_t>(ite->first);
		}
		order++;
		mConfig.mScenesOrder.try_emplace(static_cast<uint32_t>(order), sceneName);
		mScenes.try_emplace(sceneName, sceneName, static_cast<uint32_t>(order));
	}

	void SceneHandler::removeScene(const std::experimental::filesystem::path& kPath)
	{
		std::string sceneName { sceneFromPath(kPath) };
		if (mScenes.count(sceneName) == 0)
		{
			LOG(LogWarning, "failed to remove scene: scene \'" << sceneName << "\' not found");
			return;
		}

		mConfig.mScenesOrder.erase(mScenes.at(sceneName).order());
		mScenes.erase(sceneName);
	}

	void SceneHandler::update(float deltaTime, bool isSimulation)
	{
		if (mFunc)
		{
			mFunc();
			mFunc = nullptr;
		}

		if (mActiveScene)
			mActiveScene->update(deltaTime, isSimulation);
	}

	void SceneHandler::reset()
	{
		if (!mActiveScene)
			return;

		mFunc = [this]() -> void
		{
			mActiveScene->removeActors();

			Service::get<rhi::Renderer>()->frameGraph().clear();

			std::unique_ptr<JsonNode> root { JsonNode::parse((mActiveScene->name() + ".scene").c_str()) };
			mActiveScene->load(root.get());
			mActiveScene->initialize();
		};
	}

	SceneHandler::SceneConfig& SceneHandler::config()
	{
		return mConfig;
	}
}