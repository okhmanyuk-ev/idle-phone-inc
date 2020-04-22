#pragma once

#include "building_upgrade_window.h"

namespace PhoneInc
{
	class WarehouseUpgradeWindow : public BuildingWindow
	{
	public:
		WarehouseUpgradeWindow();

	protected:
		utf8_string getTitle() const override;
		utf8_string getBuildingName() const override;
		int getLevel() const override;
		std::shared_ptr<Renderer::Texture> getBuildingTexture() const override;
		double getUpgradePrice() const override;
		Parameter getFirstParameter() const override;
		Parameter getSecondParameter() const override;
	};
}
