#pragma once

#include <shared/all.h>
#include "profile.h"
#include "cheats.h"
#include "screens/loading_screen.h"
#include "screens/gameplay_screen.h"
#include "helpers.h"
#include "tutor.h"
#include "microtasks.h"

namespace PhoneInc
{
	class Application : public Shared::Application,
		public Common::FrameSystem::Frameable,
		public Common::EventSystem::Listenable<Profile::ProfileClearedEvent>
	{
	public:
		Application();
		~Application();

	private:
		void frame() override;
		void adaptToScreen(std::shared_ptr<Scene::Node> node, const glm::vec2& dimensions);

		void initializeScene();

	public:
		void event(const Profile::ProfileClearedEvent& e) override;
	};
}
