#pragma once

#include <shared/all.h>
#include "profile.h"
#include "balance.h"
#include "helpers.h"

namespace PhoneInc
{
	class TopMenu : public Scene::Sprite,
		public Common::EventSystem::Listenable<Profile::CashChangedEvent>
	{
	public:
		TopMenu();

	public:
		void event(const Profile::CashChangedEvent& e) override;

	private:
		void refreshCashLabel();

	private:
		std::shared_ptr<Helpers::LabelSolid> mCashLabel = nullptr;
	};
}