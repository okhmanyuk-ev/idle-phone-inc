#include "shop_upgrade_window.h"

using namespace PhoneInc;

ShopUpgradeWindow::ShopUpgradeWindow()
{
	refresh();
}

utf8_string ShopUpgradeWindow::getTitle() const
{
	return LOCALIZE("SHOP_WINDOW_TITLE");
}

utf8_string ShopUpgradeWindow::getBuildingName() const 
{
	return LOCALIZE(fmt::format("SHOP_NAME_{}", getLevel()));
}

int ShopUpgradeWindow::getLevel() const
{
	return PROFILE->getShopLevel();
}

std::shared_ptr<Renderer::Texture> ShopUpgradeWindow::getBuildingTexture() const
{
	return TEXTURE(fmt::format("textures/shop/{}.png", getLevel()));
}

double ShopUpgradeWindow::getUpgradePrice() const
{
	return 123.0;
}

BuildingWindow::Parameter ShopUpgradeWindow::getFirstParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("SHOP_WINDOW_PARAM_NAME_1");
	result.effect_text = "+ 10%";
	result.icon_texture = TEXTURE("textures/windows/shop_window/icon1.png");
	return result;
}

BuildingWindow::Parameter ShopUpgradeWindow::getSecondParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("SHOP_WINDOW_PARAM_NAME_2");
	result.effect_text = "$ 32.1K";
	result.icon_texture = TEXTURE("textures/windows/shop_window/icon2.png");
	return result;
}