#pragma once

#include <shared/all.h>

namespace PhoneInc::Balance
{
	const inline double StartCash = 40.0;

	const inline int MaxWarehouseLevel = 100;
	const inline int WarehouseLevelsPerStage = 10;

	const inline int MaxRooms = 30;

	const inline int MaxProductLevel = 75;
	const inline int MaxManagerLevel = 50;
	const inline int MaxWorkerLevel = 50;

	const inline int ProductLevelsPerStage = 5;
	const inline int ManagerLevelsPerStage = 10;
	const inline int WorkerLevelsPerStage = 10;

	const inline int MaxWorkersCount = 3;

	const inline int PhonesStackCount = 8;

	const inline float ManagerMinDuration = 1.0f;
	const inline float ManagerMaxDuration = 5.0f;

	const inline float WorkerMinDuration = 0.25f;
	const inline float WorkerMaxDuration = 2.0f;

	const inline float WarehouseMinDuration = 0.25f;
	const inline float WarehouseMaxDuration = 5.0f;

	double GetRoomCost(int index);

	double GetRoomProductCost(int room_index, int product_level);
	double GetRoomManagerCost(int room_index, int manager_level);
	double GetRoomWorkerCost(int room_index, int worker_level, int worker_index);
	double GetRoomProduceCount(int room_index, int product_level);

	int GetStage(int level, int levels_per_stage);

	int GetProductStage(int level);
	int GetManagerStage(int level);
	int GetWorkerStage(int level);

	int GetWarehouseStage();
	int GetTruckStage();

	double GetWarehouseCost();

	float GetWarehouseDuration();
	float GetWarehouseTruckDuration();
	double GetWarehouseTruckCapacity();
	double GetWarehouseEarning();

	float GetManagerDuration(int room_index);
	float GetWorkerDuration(int room_index, int worker_index);
}