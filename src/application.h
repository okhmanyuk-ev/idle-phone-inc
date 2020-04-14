#pragma once

#include <shared/all.h>
#include "defines.h"
#include "profile.h"
#include "cheats.h"

namespace phoneinc
{
	class Application : public Shared::RichApplication
	{
	public:
		Application();
		~Application();

	private:
		void loading(const std::string& stage, float progress) override;
		void initialize() override;
		void frame() override;

	private:
	//	Shared::SceneHelpers::SplashScene mSplashScene = Shared::SceneHelpers::SplashScene(TEXTURE("textures/logo.png"));
		Scene::Scene mGameScene;

	private:
		bool mNodeEditor = false;

	private:
		Shared::SceneEditor mSceneEditor = Shared::SceneEditor(mGameScene);
	};
}
