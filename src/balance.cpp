#include "balance.h"

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