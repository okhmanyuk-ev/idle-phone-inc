#pragma once

#include <shared/all.h>
#include "defines.h"
#include "profile.h"
#include "cheats.h"
#include "screens/gameplay.h"
#include "helpers.h"
#include "tutor.h"

namespace PhoneInc
{
	class Application : public Shared::RichApplication,
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
		float getScaleFactor(bool horizontal_priority);

	public:
		void event(const Profile::ProfileClearedEvent& e) override;

	private:
		Scene::Scene mLoadingScene;
		std::shared_ptr<Helpers::StreetProgressbar> mProgressbar;
		Scene::Scene mGameScene;
		std::shared_ptr<TutorHolder> mTutorHolder;

	private:
		Shared::SceneEditor mSceneEditor = Shared::SceneEditor(mGameScene);
		std::shared_ptr<Gameplay> mGameplay;
	};
}
