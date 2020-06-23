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
		void adaptToScreen(std::shared_ptr<Scene::Node> node, bool horizontal_priority);

		void initializeScene();

	public:
		void event(const Profile::ProfileClearedEvent& e) override;

	private:
		Scene::Scene mGameScene;

	private:
		Shared::SceneEditor mSceneEditor = Shared::SceneEditor(mGameScene);
		bool mSceneInitialized = false;
	};
}
