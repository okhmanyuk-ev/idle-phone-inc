#include "shop_window.h"

using namespace PhoneInc;

ShopWindow::ShopWindow()
{
	refresh();
}

utf8_string ShopWindow::getTitle() const
{
	return LOCALIZE("SHOP_WINDOW_TITLE");
}

utf8_string ShopWindow::getBuildingName() const
{
	return LOCALIZE(fmt::format("SHOP_NAME_{}", getLevel()));
}

int ShopWindow::getLevel() const
{
	return PROFILE->getShopLevel();
}

std::shared_ptr<Renderer::Texture> ShopWindow::getBuildingTexture() const
{
	return TEXTURE(fmt::format("textures/shop/{}.png", getLevel()));
}

double ShopWindow::getUpgradePrice() const
{
	return 123.0;
}

BuildingWindow::Parameter ShopWindow::getFirstParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("SHOP_WINDOW_PARAM_NAME_1");
	result.effect_text = "+ 10%";
	result.icon_texture = TEXTURE("textures/windows/shop_window/icon1.png");
	return result;
}

BuildingWindow::Parameter ShopWindow::getSecondParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("SHOP_WINDOW_PARAM_NAME_2");
	result.effect_text = "$ 32.1K";
	result.icon_texture = TEXTURE("textures/windows/shop_window/icon2.png");
	return result;
}