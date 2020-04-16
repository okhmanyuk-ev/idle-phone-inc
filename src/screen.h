#pragma once

#include <shared/all.h>

namespace PhoneInc
{
	class Screen : public Shared::SceneManager::Screen, public std::enable_shared_from_this<Screen>
	{
	public:
		Screen()
		{
			setEnabled(false);
			setInteractions(false);
			setStretch(1.0f);
			setAnchor(0.5f);
			setPivot(0.5f);
		//	setAlpha(0.0f);
		}

	protected:
		void onEnterBegin() override
		{
			setEnabled(true);
		}

		void onEnterEnd() override
		{
			setInteractions(true);
		}

		void onLeaveBegin() override
		{
			setInteractions(false);
		}

		void onLeaveEnd() override
		{
			setEnabled(false);
		//	setAlpha(0.0f);
		}

		void onWindowAppearing() override
		{
			setInteractions(false);
		}

		void onWindowDisappearing() override
		{
			setInteractions(true);
		}

		std::unique_ptr<Common::Actions::Action> createEnterAction() override
		{
		//	return Shared::ActionHelpers::Show(shared_from_this(), 0.25f);
			return nullptr;
		};

		std::unique_ptr<Common::Actions::Action> createLeaveAction() override
		{
		//	return Shared::ActionHelpers::Hide(shared_from_this(), 0.25f);
			return nullptr;
		};
	};
}