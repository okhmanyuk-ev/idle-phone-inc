#include "shop_upgrade_window.h"

using namespace PhoneInc;

ShopUpgradeWindow::ShopUpgradeWindow()
{
	refresh();
}

utf8_string ShopUpgradeWindow::getTitle() const
{
	return LOCALIZE("SHOP_UPGRADE_WINDOW_TITLE");
}

int ShopUpgradeWindow::getLevel() const
{
	return PROFILE->getShopLevel();
}