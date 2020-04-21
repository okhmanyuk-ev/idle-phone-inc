#pragma once

#include "building_upgrade_window.h"

namespace PhoneInc
{
	class ShopUpgradeWindow : public BuildingUpgradeWindow
	{
	public:
		ShopUpgradeWindow();

	protected:
		utf8_string getTitle() const override;
		int getLevel() const override;
	};
}
