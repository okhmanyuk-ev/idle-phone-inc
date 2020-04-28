#pragma once

#include <shared/all.h>

namespace PhoneInc::Balance
{
	const inline int MaxProductLevel = 60;
	const inline int MaxManagerLevel = 50;
	const inline int MaxWorkerLevel = 50;

	const inline int ProductLevelsPerStage = 10;
	const inline int ManagerLevelsPerStage = 10;
	const inline int WorkerLevelsPerStage = 10;

	const inline int MaxWarehouseStorage = 10;


	double GetRoomCost(int index);
	double GetProductCost(int index);
	double GetManagerCost(int index);
	double GetWorkerCost(int index);

	int GetProductStageFromLevel(int level);
	int GetManagerStageFromLevel(int level);
	int GetWorkerStageFromLevel(int level);
}