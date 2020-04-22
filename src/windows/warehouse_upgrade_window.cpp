#include "warehouse_upgrade_window.h"

using namespace PhoneInc;

WarehouseUpgradeWindow::WarehouseUpgradeWindow()
{
	refresh();
}

utf8_string WarehouseUpgradeWindow::getTitle() const
{
	return LOCALIZE("WAREHOUSE_WINDOW_TITLE");
}

utf8_string WarehouseUpgradeWindow::getBuildingName() const
{
	return LOCALIZE(fmt::format("WAREHOUSE_NAME_{}", getLevel()));
}

int WarehouseUpgradeWindow::getLevel() const
{
	return PROFILE->getWarehouseLevel();
}

std::shared_ptr<Renderer::Texture> WarehouseUpgradeWindow::getBuildingTexture() const
{
	return TEXTURE(fmt::format("textures/warehouse/{}.png", getLevel()));
}

double WarehouseUpgradeWindow::getUpgradePrice() const
{
	return 123.0;
}

BuildingWindow::Parameter WarehouseUpgradeWindow::getFirstParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("WAREHOUSE_WINDOW_PARAM_NAME_1");
	result.effect_text = "+ 10%";
	result.icon_texture = TEXTURE("textures/windows/warehouse_window/icon1.png");
	return result;
}

BuildingWindow::Parameter WarehouseUpgradeWindow::getSecondParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("WAREHOUSE_WINDOW_PARAM_NAME_2");
	result.effect_text = "+ 5%";
	result.icon_texture = TEXTURE("textures/windows/warehouse_window/icon2.png");
	return result;
}