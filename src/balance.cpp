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

int Balance::GetProductStageFromLevel(int level)
{
	return ((level - 1) / Balance::ProductLevelsPerStage) + 1;
}

int Balance::GetManagerStageFromLevel(int level)
{
	return ((level - 1) / Balance::ManagerLevelsPerStage) + 1;
}

int Balance::GetWorkerStageFromLevel(int level)
{
	return ((level - 1) / Balance::WorkerLevelsPerStage) + 1;
}