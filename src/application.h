#pragma once

#include <shared/all.h>
#include "defines.h"
#include "profile.h"
#include "cheats.h"
#include "screens/gameplay.h"
#include "helpers.h"

namespace PhoneInc
{
	class Application : public Shared::RichApplication,
		public Common::EventSystem::Listenable<Helpers::PushWindowEvent>,
		public Common::EventSystem::Listenable<Helpers::PopWindowEvent>,
		public Common::EventSystem::Listenable<Profile::ProfileClearedEvent>
	{
	public:
		Application();
		~Application();

	private:
		void loading(const std::string& stage, float progress) override;
		void initialize() override;
		void prepare();
		void frame() override;
		void makeLoadingScene();
		void updateGameScale();
		void updateLoadingScale();
		float getScaleFactor(float max_ratio);

	public:
		void event(const Helpers::PushWindowEvent& e) override;
		void event(const Helpers::PopWindowEvent& e) override;
		void event(const Profile::ProfileClearedEvent& e) override;

	private:
		Scene::Scene mLoadingScene;
		std::shared_ptr<Helpers::StreetProgressbar> mProgressbar;
		Scene::Scene mGameScene;
		std::shared_ptr<Shared::SceneManager> mSceneManager;

	private:
		Shared::SceneEditor mSceneEditor = Shared::SceneEditor(mGameScene);
		std::shared_ptr<Gameplay> mGameplay;
	};
}
