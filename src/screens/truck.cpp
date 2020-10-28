#include "truck.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Street::Truck::Truck() : Scene::MappedAniSprite<TruckAnimation>(Animations)
{
	setFrequency(8.0f);
	setStateType(Animation::Main);

	auto stage = Balance::GetTruckStage();
	
	auto texture = TEXTURE(fmt::format("textures/trucks/{}.png", stage));
	auto atlas = ATLAS(fmt::format("textures/trucks/{}_atlas.json", stage));
	auto animation = ANIMATION(fmt::format("textures/trucks/{}_animation.json", stage));

	setAnimation(texture, atlas, animation);

}