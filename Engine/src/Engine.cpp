#include "Engine.h"

#include <Logging.h>

namespace maj
{
	DEFINE_HASH_OVERLOAD(Engine::Config)

	Engine::Config::Config()
		:mViewport{ math::Vec2ui{0u,0u}, "MAJEngine" }
	{
		std::unique_ptr<JsonNode> root{ JsonNode::parse("Engine.config") };
		load(root.get());
	}
	
	Engine::Config::~Config()
	{
		save("Engine.config");
	}


	Engine::Engine()
		: Service { typeid(Engine).name() }, mThreadPool {}, mWindow { mConfig.mViewport }, mInstance { mConfig.mExtensions }, mIsSimulating { true }
	{
#ifdef MAJ_EDITOR
		mIsSimulating = false;
		mOnStartSimulation += [this] ()
		{
			scene::Scene* activeScene{ Service::get<scene::SceneHandler>()->activeScene() };
			if (activeScene)
				activeScene->save((std::string{ activeScene->name() } +".scene").c_str());

			mIsSimulating = true;
		};
		mOnStopSimulation += [this] ()
		{
			Service::get<scene::SceneHandler>()->reset();
			mIsSimulating = false;
		};
#endif

#ifndef MAJ_EDITOR
		mAssetHandler.load(".");
		mScenes.load(".");

		mScenes.activeScene(0u);
#endif

		mPhysicTimer.set(0.02f, [&] () -> void
						 {
							 scene::Scene* activeScene { mScenes.activeScene() };
							 if (!mIsSimulating || !activeScene)
								 return;

							 activeScene->fixedUpdate();
							 mPhysicInstance.scene()->simulate(0.02f);
							 mPhysicInstance.scene()->fetchResults(true);
						 });
		mPhysicTimer.isLoop(true);

#ifdef _DEBUG
		mFps = 0u;
		mDebugTimer.set(1.f, [&] () -> void
						{
							uint32_t latency { static_cast<uint32_t>(mTimer.duration<std::micro>()) };
							LOG(LogVoid, std::to_string(latency / 1000u) << '.' << std::to_string(latency % 1000u) << " ms (FPS " << std::to_string(mFps) << ')')
							mFps = 0u;
						});
		mDebugTimer.isLoop(true);
		mDebugTimer.start();
#endif

		mPhysicTimer.start();
	}

	Engine::~Engine()
	{}

	void Engine::startFrame()
	{
#ifdef _DEBUG
		mFps++;
		mDebugTimer.step();
#endif

		float dt { mTimer.duration<std::ratio<1, 1>>() };
		mTimer.start();

#ifndef MAJ_EDITOR
		mWindow.pollEvents();
#else
		if (Service::get<ww::Input>()->key(ww::EKey::F5).isPressed())
		{
			if (mIsSimulating)
				mOnStopSimulation();
			else if(mScenes.activeScene() && mScenes.activeScene()->cameras().count() >= 2u)
				mOnStartSimulation();
		}
#endif

		mInstance.update();

		mPhysicTimer.step();
		mScenes.update(dt, mIsSimulating);
	}

	void Engine::endFrame()
	{
		mInstance.draw();
		mTimer.stop();
	}

	bool Engine::isRunning() const
	{
		return mWindow.isAnyOpen();
	}

	bool Engine::isSimulating() const
	{
		return mIsSimulating;
	}

	Engine::Config& Engine::config()
	{
		return mConfig;
	}
}

