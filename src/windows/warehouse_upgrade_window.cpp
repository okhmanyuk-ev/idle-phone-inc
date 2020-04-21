#include "warehouse_upgrade_window.h"

using namespace PhoneInc;

WarehouseUpgradeWindow::WarehouseUpgradeWindow()
{
	refresh();
}

utf8_string WarehouseUpgradeWindow::getTitle() const
{
	return LOCALIZE("WAREHOUSE_UPGRADE_WINDOW_TITLE");
}

int WarehouseUpgradeWindow::getLevel() const
{
	return PROFILE->getWarehouseLevel();
}