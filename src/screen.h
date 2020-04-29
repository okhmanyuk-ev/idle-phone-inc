#pragma once

#include <shared/all.h>

namespace PhoneInc
{
	class Screen : public Shared::SceneManager::Screen, public std::enable_shared_from_this<Screen>
	{
	public:
		Screen();

	protected:
		void onEnterBegin() override;
		void onEnterEnd() override;
		void onLeaveBegin() override;
		void onLeaveEnd() override;
		void onWindowAppearing() override;
		void onWindowDisappearing() override;

	protected:
		std::unique_ptr<Common::Actions::Action> createEnterAction() override;
		std::unique_ptr<Common::Actions::Action> createLeaveAction() override;
	};
}