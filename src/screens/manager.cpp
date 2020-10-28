#include "manager.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Factory::Manager::Manager() : Scene::MappedAniSprite<ManagerAnimation>(Animations)
{
	setFrequency(6.0f);
	setStateType(Animation::Idle);

	mProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mProgressbar->setSize({ 96.0f, 12.0f });
	mProgressbar->setPivot(0.5f);
	mProgressbar->setAnchor({ 0.5f, 0.0f });
	mProgressbar->setY(-8.0f);
	mProgressbar->setProgress(0.0f);
	mProgressbar->setScale(0.0f);
	mProgressbar->setEnabled(false);
	attach(mProgressbar);
}

void Factory::Manager::refresh()
{
	if (mLevel <= 0)
	{
		setEnabled(false);
		return;
	}

	setEnabled(true);

	auto stage = Balance::GetManagerStage(mLevel);
	auto name = fmt::format("manager_{}", stage);
	
	auto load_atlas = [stage] {
		auto texture_path = fmt::format("textures/managers/{}.png", stage);
		auto atlas_path = "textures/managers/atlas.json";
		return std::make_shared<Graphics::Atlas>(TEXTURE(texture_path), Platform::Asset(atlas_path));
	};
	
	auto load_animation = [name, load_atlas] {
		auto atlas = ATLAS(name, load_atlas);
		auto animation_path = "textures/managers/animation.json";
		return std::make_shared<Graphics::Animation>(atlas, Platform::Asset(animation_path));
	};

	setAnimation(ANIMATION(name, load_animation));

	//randomizeProgress();
}

void Factory::Manager::setLevel(int value)
{
	mLevel = value;
	refresh();
}