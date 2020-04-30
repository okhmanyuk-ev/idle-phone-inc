#pragma once

#include <shared/all.h>

namespace PhoneInc::Balance
{
	const inline int MaxWarehouseLevel = 40;
	const inline int WarehouseLevelsPerStage = 10;

	const inline int MaxRooms = 30;

	const inline int MaxProductLevel = 60;
	const inline int MaxManagerLevel = 50;
	const inline int MaxWorkerLevel = 50;

	const inline int ProductLevelsPerStage = 10;
	const inline int ManagerLevelsPerStage = 10;
	const inline int WorkerLevelsPerStage = 10;

	double GetRoomCost(int index);
	double GetProductCost(int index);
	double GetManagerCost(int index);
	double GetWorkerCost(int index);

	int GetStage(int level, int levels_per_stage);

	int GetProductStage(int level);
	int GetManagerStage(int level);
	int GetWorkerStage(int level);

	int GetWarehouseStage();

	double GetWarehouseCost();

	float GetWarehouseDurationMultiplier();
	float GetWarehouseDuration();
}