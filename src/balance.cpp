#include "balance.h"

using namespace PhoneInc;

double Balance::GetRoomCost(int number)
{
	const double BaseCost = 10;
	return BaseCost * glm::pow(3.2, (double)number);
}