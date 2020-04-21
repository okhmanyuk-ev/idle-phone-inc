#pragma once

#include "building_upgrade_window.h"

namespace PhoneInc
{
	class WarehouseUpgradeWindow : public BuildingUpgradeWindow
	{
	public:
		WarehouseUpgradeWindow();

	protected:
		utf8_string getTitle() const override;
		int getLevel() const override;
	};
}
