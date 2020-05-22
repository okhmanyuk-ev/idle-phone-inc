#include "balance.h"
#include "profile.h"

using namespace PhoneInc;

double Balance::GetRoomCost(int index)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)index);
}

double Balance::GetRoomProductCost(int room_index, int product_level)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)product_level) * glm::pow(3.2, (double)room_index);
}

double Balance::GetRoomManagerCost(int room_index, int manager_level)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)manager_level) * glm::pow(3.2, (double)room_index);
}

double Balance::GetRoomWorkerCost(int room_index, int worker_level)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)worker_level) * glm::pow(3.2, (double)room_index);
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

int Balance::GetTruckStage()
{
	return ((Balance::GetWarehouseStage() - 1) / 2) + 1;
}

double Balance::GetWarehouseCost()
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(3.2, (double)PROFILE->getWarehouseLevel());
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

double Balance::GetWarehouseTruckCapacity()
{
	const double BaseCapacity = 10.0;
	return BaseCapacity * glm::pow(3.2, (double)PROFILE->getWarehouseLevel() - 1);
}

double Balance::GetWarehouseEarning()
{
	const double BaseEarning = 10.0;
	return BaseEarning * glm::pow(3.2, (double)PROFILE->getWarehouseLevel() - 1);
}

float Balance::GetManagerDuration(int room_index)
{
	float MinDuration = 1.0f;
	float MaxDuration = 5.0f;

	auto& room = PROFILE->getRooms().at(room_index);
	auto lvl = room.manager;

	return glm::lerp(MaxDuration, MinDuration, (float)lvl / (float)MaxManagerLevel);
}

float Balance::GetWorkerDuration(int room_index, int worker_index)
{
	float MinDuration = 0.25f;
	float MaxDuration = 2.0f;

	auto& room = PROFILE->getRooms().at(room_index);
	auto lvl = room.workers.at(worker_index);
	
	return glm::lerp(MaxDuration, MinDuration, (float)lvl / (float)MaxWorkerLevel);
}