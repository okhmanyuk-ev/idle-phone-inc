#pragma once

#include <shared/all.h>

namespace PhoneInc::Balance
{
	const inline int MaxWarehouseLevel = 50;
	const inline int WarehouseLevelsPerStage = 5;

	const inline int MaxRooms = 30;

	const inline int MaxProductLevel = 150;
	const inline int MaxManagerLevel = 50;
	const inline int MaxWorkerLevel = 50;

	const inline int ProductLevelsPerStage = 10;
	const inline int ManagerLevelsPerStage = 10;
	const inline int WorkerLevelsPerStage = 10;

	const inline int MaxWorkersCount = 3;

	double GetRoomCost(int index);
	double GetProductCost(int index);
	double GetManagerCost(int index);
	double GetWorkerCost(int index);

	int GetStage(int level, int levels_per_stage);

	int GetProductStage(int level);
	int GetManagerStage(int level);
	int GetWorkerStage(int level);

	int GetWarehouseStage();
	int GetTruckStage();

	double GetWarehouseCost();

	float GetWarehouseDurationMultiplier();
	float GetWarehouseDuration();

	float GetManagerDuration(int room_index);
	float GetWorkerDuration(int room_index, int worker_index);
}