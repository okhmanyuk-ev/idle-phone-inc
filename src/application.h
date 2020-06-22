#pragma once

#include <shared/all.h>
#include "defines.h"
#include "profile.h"
#include "cheats.h"
#include "screens/loading_screen.h"
#include "screens/gameplay_screen.h"
#include "helpers.h"
#include "tutor.h"

namespace PhoneInc
{
	class Application : public Shared::RichApplication,
		public Common::FrameSystem::Frameable,
		public Common::EventSystem::Listenable<Profile::ProfileClearedEvent>
	{
	public:
		Application();
		~Application();

	private:
		void frame() override;
		float getScaleFactor(bool horizontal_priority);
		void adaptToScreen(std::shared_ptr<Scene::Node> node, bool horizontal_priority);

		void initializeScene();

	public:
		void event(const Profile::ProfileClearedEvent& e) override;

	private:
	//	Scene::Scene mLoadingScene;
		std::shared_ptr<Helpers::StreetProgressbar> mProgressbar;
		Scene::Scene mGameScene;
		std::shared_ptr<TutorHolder> mTutorHolder;

	private:
		Shared::SceneEditor mSceneEditor = Shared::SceneEditor(mGameScene);
		std::shared_ptr<GameplayScreen> mGameplayScreen;
	};
}
