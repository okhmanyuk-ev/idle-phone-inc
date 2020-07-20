#pragma once

#include "screen.h"
#include "tutor.h"
#include "helpers.h"
#include "microtasks.h"
#include "microtasks_holder.h"

namespace PhoneInc
{
	class GameplayScreen : public Scene::Actionable<Screen>,
		public Common::EventSystem::Listenable<Helpers::MoveGlobalScrollEvent>,
		public Common::EventSystem::Listenable<Helpers::BlockGlobalScrollEvent>
	{
	public:
		GameplayScreen();

	public:
		void event(const Helpers::MoveGlobalScrollEvent& e) override;
		void event(const Helpers::BlockGlobalScrollEvent& e) override;

	private:
		std::shared_ptr<Scene::Scrollbox> mScrollbox;
	};
}