#include "truck.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Street::Truck::Truck() : Scene::MappedAniSprite<TruckAnimation>(Animations)
{
	setFrequency(8.0f);
	setStateType(Animation::Main);

	auto texture = TEXTURE(fmt::format("textures/trucks/{}.png", Balance::GetTruckStage()));
	auto animation = ANIMATION(fmt::format("textures/trucks/{}", Balance::GetTruckStage()));
	setAnimation(texture, animation);
}