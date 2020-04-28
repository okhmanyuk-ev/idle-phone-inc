#include "truck.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Street::Truck::Truck() : Scene::MappedAniSprite<TruckAnimation>(Animations)
{
	setFrequency(8.0f);
	setStateType(Animation::Main);

	auto texture = TEXTURE("textures/trucks/1.png");
	auto animation = ANIMATION("textures/trucks/1");
	setAnimation(texture, animation);

}