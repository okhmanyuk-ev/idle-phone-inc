#pragma once

#include <shared/all.h>
#include "profile.h"
#include "balance.h"
#include "helpers.h"

namespace PhoneInc
{
	class TopMenu : public Scene::Sprite,
		public Common::Event::Listenable<Profile::CashChangedEvent>,
        public Common::Event::Listenable<Profile::CoinsChangedEvent>
	{
	public:
		TopMenu();

	public:
		void onEvent(const Profile::CashChangedEvent& e) override;
        void onEvent(const Profile::CoinsChangedEvent& e) override;

	private:
		void refreshCashLabel();
        void refreshCoinsLabel();

	private:
		std::shared_ptr<Helpers::LabelSolid> mCashLabel = nullptr;
        std::shared_ptr<Helpers::LabelSolid> mCoinsLabel = nullptr;
	};
}
