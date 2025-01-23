#pragma once

#include <sky/sky.h>
#include "profile.h"
#include "cheats.h"
#include "screens/loading_screen.h"
#include "screens/gameplay_screen.h"
#include "helpers.h"
#include "tutor.h"
#include "microtasks.h"

namespace PhoneInc
{
	class Application : public sky::Scheduler::Frameable,
		public sky::Listenable<Profile::ProfileClearedEvent>
	{
	public:
		Application();
		~Application();

	private:
		void onFrame() override;
		void initializeScene();

	public:
		void onEvent(const Profile::ProfileClearedEvent& e) override;
	};
}
