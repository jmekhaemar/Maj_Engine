#ifndef _MAJ_ENGINE_H_
#define _MAJ_ENGINE_H_

#include <Timer.h>
#include <ThreadPool.h>
#include <ScriptModule.h>
#include <Instance.h>
#include <Window.h>

#include "physic/Instance.h"
#include "resources/AssetsHandler.h"
#include "scene/SceneHandler.h"

namespace maj
{
	class Engine final : public Service
	{
	public:
		struct Config final : public ISerializable
		{
			ENABLE_META(Config)

		public:
			Config();
			~Config();

			ww::Viewport::Info mViewport;
			rhi::ExtensionList mExtensions;
		};

	public:
		Engine();
		~Engine();

		void startFrame();
		void endFrame();

		bool isRunning() const;
		bool isSimulating() const;
		Config& config();

#ifdef MAJ_EDITOR
		Event<void> mOnStartSimulation;
		Event<void> mOnStopSimulation;
#endif

	private:
		Config mConfig;
		ThreadPool mThreadPool;
		ww::Window mWindow;
		rhi::Instance mInstance;
		physic::Instance mPhysicInstance;
		resources::AssetsHandler mAssetHandler;
		ScriptModule mScript;
		scene::SceneHandler mScenes;
		
		Timer mTimer;
		Timer mPhysicTimer;

#ifdef _DEBUG
		Timer mDebugTimer;
		uint16_t mFps;
#endif

		bool mIsSimulating;
	};
}

META_CLASS(maj::Engine::Config, 2u)
META_FIELD(maj::Engine::Config, maj::ww::Viewport::Info, mViewport, 0u)
META_FIELD(maj::Engine::Config, maj::rhi::ExtensionList, mExtensions, 1u)

#endif