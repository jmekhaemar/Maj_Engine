#ifndef _SCENE_HANDLER_H_
#define _SCENE_HANDLER_H_

#include <unordered_map>
#include <experimental/filesystem>

#include <Service.h>

#include "scene/Scene.h"

#ifdef MAJ_EDITOR
#include "gizmo/GizmoHandler.h"
#endif

namespace maj::scene
{
	class SceneHandler final : public Service
	{
	public:
		struct SceneConfig final : public ISerializable
		{
			ENABLE_META(SceneConfig)

		public:
			LOAD_CONSTRUCTOR(SceneConfig);

			SceneConfig() = default;
			~SceneConfig() = default;

			std::unordered_map<uint32_t, std::string> mScenesOrder;
		};

		static std::string sceneFromPath(const std::experimental::filesystem::path& kPath);

	public:
		SceneHandler();
		~SceneHandler();

		void load(const char* kRootPath);

		const Scene* activeScene() const;
		Scene* activeScene();
		void activeScene(const std::experimental::filesystem::path& kPath);
		void activeScene(uint16_t order);

		const Scene* scene(const char* kStr) const;
		Scene* scene(const char* kStr);

		void rename(const std::experimental::filesystem::path& kOldPath, const std::experimental::filesystem::path& kNewPath);
		void addScene(const std::experimental::filesystem::path& kPath);
		void removeScene(const std::experimental::filesystem::path& kPath);

		void update(float deltaTime, bool isSimulation);
		void reset();
		SceneConfig& config();
	private:
#ifdef MAJ_EDITOR
		gizmo::GizmoHandler mGizmos;
#endif

		SceneConfig mConfig;
		//std::unordered_map<uint16_t, std::string> mScenesOrder;
		std::unordered_map<std::string, Scene> mScenes;

		Scene* mActiveScene;
		std::function<void()> mFunc;
	};
}

typedef std::unordered_map<uint32_t, std::string> SceneOrderMap;

META_CLASS(maj::scene::SceneHandler::SceneConfig, 1u)
META_FIELD_MAP(maj::scene::SceneHandler::SceneConfig, SceneOrderMap, mScenesOrder, 0u)

#endif