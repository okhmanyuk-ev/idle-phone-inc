#include "truck.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Street::Truck::Truck()
{
	setFrequency(8.0f);
	setStateType(Animation::Main);

	auto stage = Balance::GetTruckStage();

	auto texture = sky::GetTexture(fmt::format("textures/trucks/{}.png", stage));
	auto atlas = sky::GetAtlas(fmt::format("textures/trucks/{}_atlas.json", stage));
	auto animation = sky::GetAnimation(fmt::format("textures/trucks/{}_animation.json", stage));

	setAnimation(texture, atlas, animation);
}