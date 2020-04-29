#pragma once

#include "building_window.h"

namespace PhoneInc
{
	class ShopWindow : public BuildingWindow,
		public Common::EventSystem::Listenable<Profile::ShopLevelChangedEvent>
	{
	public:
		ShopWindow();

	private:
		void event(const Profile::ShopLevelChangedEvent& e) override;

	protected:
		utf8_string getTitle() const override;
		utf8_string getBuildingName() const override;
		int getLevel() const override;
		int getMaxLevel() const override;
		std::shared_ptr<Renderer::Texture> getBuildingTexture() const override;
		double getUpgradePrice() const override;
		Parameter getFirstParameter() const override;
		Parameter getSecondParameter() const override;
		void upgrade() override;
	};
}
