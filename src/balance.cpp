#include "balance.h"
#include "profile.h"

using namespace PhoneInc;

double Balance::GetRoomCost(int index)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)index);
}

double Balance::GetProductCost(int index)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)index);
}

double Balance::GetManagerCost(int index)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)index);
}

double Balance::GetWorkerCost(int index)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)index);
}

int Balance::GetStage(int level, int levels_per_stage)
{
	return ((level - 1) / levels_per_stage) + 1;
}

int Balance::GetProductStage(int level)
{
	return GetStage(level, ProductLevelsPerStage);
}

int Balance::GetManagerStage(int level)
{
	return GetStage(level, ManagerLevelsPerStage);
}

int Balance::GetWorkerStage(int level)
{
	return GetStage(level, WorkerLevelsPerStage);
}

int Balance::GetWarehouseStage()
{
	return GetStage(PROFILE->getWarehouseLevel(), WarehouseLevelsPerStage);
}

int Balance::GetShopStage()
{
	return GetStage(PROFILE->getShopLevel(), ShopLevelsPerStage);
}

double Balance::GetWarehouseCost()
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)PROFILE->getWarehouseLevel());
}

double Balance::GetShopCost()
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)PROFILE->getShopLevel());
}

float Balance::GetWarehouseDurationMultiplier()
{
	return 1.0f - (((float)PROFILE->getWarehouseLevel() / (float)MaxWarehouseLevel) * 0.95f);
}

float Balance::GetWarehouseDuration()
{
	const float BaseDuration = 10.0f;
	return BaseDuration * GetWarehouseDurationMultiplier();
}

float Balance::GetShopDurationMultiplier()
{
	return 1.0f - (((float)PROFILE->getShopLevel() / (float)MaxShopLevel) * 0.95f);
}

float Balance::GetShopDuration()
{
	const float BaseDuration = 10.0f;
	return BaseDuration * GetShopDurationMultiplier();
}