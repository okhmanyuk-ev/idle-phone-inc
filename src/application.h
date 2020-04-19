#pragma once

#include <shared/all.h>
#include "defines.h"
#include "profile.h"
#include "cheats.h"
#include "gameplay.h"

namespace PhoneInc
{
	class Application : public Shared::RichApplication
	{
	public:
		Application();
		~Application();

	private:
		void loading(const std::string& stage, float progress) override;
		void initialize() override;
		void prepare();
		void frame() override;

	private:
	//	Shared::SceneHelpers::SplashScene mSplashScene = Shared::SceneHelpers::SplashScene(TEXTURE("textures/logo.png"));
		Scene::Scene mGameScene;
		std::shared_ptr<Shared::SceneManager> mSceneManager;

	private:
		bool mNodeEditor = false;

	private:
		Shared::SceneEditor mSceneEditor = Shared::SceneEditor(mGameScene);
		std::shared_ptr<Gameplay> mGameplay;
	};
}
