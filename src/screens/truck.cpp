#include "truck.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Street::Truck::Truck() : Scene::MappedAniSprite<TruckAnimation>(Animations)
{
	setFrequency(8.0f);
	setStateType(Animation::Main);

	auto stage = (Balance::GetWarehouseStage() - 1) / 2;

	stage += 1;

	auto texture = TEXTURE(fmt::format("textures/trucks/{}.png", stage));
	auto animation = ANIMATION(fmt::format("textures/trucks/{}", stage));
	setAnimation(texture, animation);

}