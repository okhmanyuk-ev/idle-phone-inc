#include "warehouse_window.h"
#include "balance.h"

using namespace PhoneInc;

WarehouseWindow::WarehouseWindow()
{
	refresh();
}

void WarehouseWindow::event(const Profile::WarehouseLevelChangedEvent& e)
{
	refresh();
}

utf8_string WarehouseWindow::getTitle() const
{
	return LOCALIZE("WAREHOUSE_WINDOW_TITLE");
}

utf8_string WarehouseWindow::getBuildingName() const
{
	return LOCALIZE(fmt::format("WAREHOUSE_NAME_{}", Balance::GetWarehouseStage()));
}

int WarehouseWindow::getLevel() const
{
	return PROFILE->getWarehouseLevel();
}

int WarehouseWindow::getMaxLevel() const
{
	return Balance::MaxWarehouseLevel;
}

int WarehouseWindow::getLevelsPerStage() const
{
	return Balance::WarehouseLevelsPerStage;
}

std::shared_ptr<Renderer::Texture> WarehouseWindow::getBuildingTexture() const
{
	return TEXTURE(fmt::format("textures/warehouse/{}.png", Balance::GetWarehouseStage()));
}

double WarehouseWindow::getUpgradePrice() const
{
	return Balance::GetWarehouseCost();
}

BuildingWindow::Parameter WarehouseWindow::getFirstParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("WAREHOUSE_WINDOW_PARAM_NAME_1");
	result.effect_text = "+ 10%";
	result.icon_texture = TEXTURE("textures/windows/warehouse_window/icon1.png");
	return result;
}

BuildingWindow::Parameter WarehouseWindow::getSecondParameter() const
{
	auto result = BuildingWindow::Parameter();
	result.title_text = LOCALIZE("WAREHOUSE_WINDOW_PARAM_NAME_2");
	result.effect_text = "+ 5%";
	result.icon_texture = TEXTURE("textures/windows/warehouse_window/icon2.png");
	return result;
}

void WarehouseWindow::upgrade()
{
	PROFILE->setWarehouseLevel(PROFILE->getWarehouseLevel() + 1);
}