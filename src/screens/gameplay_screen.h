#pragma once

#include "screen.h"
#include "tutor.h"
#include "helpers.h"
#include "microtasks.h"
#include "microtasks_holder.h"

namespace PhoneInc
{
	class GameplayScreen : public Screen,
		public sky::Listenable<Helpers::MoveGlobalScrollEvent>,
		public sky::Listenable<Helpers::BlockGlobalScrollEvent>
	{
	public:
		GameplayScreen();

	public:
		void onEvent(const Helpers::MoveGlobalScrollEvent& e) override;
		void onEvent(const Helpers::BlockGlobalScrollEvent& e) override;

	private:
		std::shared_ptr<Scene::Scrollbox> mScrollbox;
	};
}