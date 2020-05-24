#include "balance.h"
#include "profile.h"

using namespace PhoneInc;

double Balance::GetRoomCost(int index)
{
	const double BaseCost = 10.0;
	return BaseCost * glm::pow(124.0, (double)index);
}

double Balance::GetRoomProductCost(int room_index, int product_level)
{
	auto result = 20.0;
	
	result *= glm::pow(1.3, (double)product_level);
	result *= glm::pow(3.2, (double)room_index);

	return result;
}

double Balance::GetRoomManagerCost(int room_index, int manager_level)
{
	auto result = 30.0;

	result *= glm::pow(1.2, (double)manager_level);
	result *= glm::pow(3.2, (double)room_index);

	return result;
}

double Balance::GetRoomWorkerCost(int room_index, int worker_level, int worker_index)
{
	auto result = 10.0;

	result *= glm::pow(1.2, (double)worker_level);
	result *= glm::pow(3.2, (double)room_index);
	result *= glm::pow(6.0, (double)(worker_index - 1));

	return result;
}

double Balance::GetRoomProduceCount(int room_index, int product_level)
{
	auto result = (double)PhonesStackCount;
	
	result *= glm::pow(1.2, (double)(product_level - 1));
	result *= glm::pow(1.6, (double)room_index);

	return result;
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
	const double BaseCost = 50.0;
	return BaseCost * glm::pow(1.96, (double)(PROFILE->getWarehouseLevel() - 1));
}

float Balance::GetWarehouseDuration()
{
	auto progress = (float)PROFILE->getWarehouseLevel() / (float)MaxWarehouseLevel;
	return glm::lerp(WarehouseMaxDuration, WarehouseMinDuration, progress);
}

float Balance::GetWarehouseTruckDuration()
{
	return 4.0f;
}

double Balance::GetWarehouseTruckCapacity()
{
	auto result = 4.0;

	result *= glm::pow(1.7, (double)PROFILE->getWarehouseLevel() - 1);

	return result;
}

double Balance::GetWarehouseEarning()
{
	auto result = 8.0;

	result *= glm::pow(1.3, (double)PROFILE->getWarehouseLevel() - 1);

	return result;
}

float Balance::GetManagerDuration(int room_index)
{
	auto& room = PROFILE->getRooms().at(room_index);
	auto lvl = room.manager;
	auto progress = (float)lvl / (float)MaxManagerLevel;
	return glm::lerp(ManagerMaxDuration, ManagerMinDuration, progress);
}

float Balance::GetWorkerDuration(int room_index, int worker_index)
{
	auto& room = PROFILE->getRooms().at(room_index);
	auto lvl = room.workers.at(worker_index);
	auto progress = (float)lvl / (float)MaxWorkerLevel;
	return glm::lerp(WorkerMaxDuration, WorkerMinDuration, progress);
}