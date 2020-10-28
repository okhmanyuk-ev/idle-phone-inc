#include "truck.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Street::Truck::Truck() : Scene::MappedAniSprite<TruckAnimation>(Animations)
{
	setFrequency(8.0f);
	setStateType(Animation::Main);

	auto stage = Balance::GetTruckStage();
	auto name = fmt::format("truck_{}", stage);

	auto load_atlas = [stage] {
		auto texture_path = fmt::format("textures/trucks/{}.png", stage);
		auto atlas_path = fmt::format("textures/trucks/{}_atlas.json", stage);
		return std::make_shared<Graphics::Atlas>(TEXTURE(texture_path), Platform::Asset(atlas_path));
	};

	auto load_animation = [name, stage, load_atlas] {
		auto atlas = ATLAS(name, load_atlas);
		auto animation_path = fmt::format("textures/trucks/{}_animation.json", stage);
		return std::make_shared<Graphics::Animation>(atlas, Platform::Asset(animation_path));
	};

	setAnimation(ANIMATION(name, load_animation));
}