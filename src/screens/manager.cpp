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

	auto texture = sky::GetTexture(fmt::format("textures/managers/{}.png", stage));
	auto atlas = sky::GetAtlas("textures/managers/atlas.json");
	auto animation = sky::GetAnimation("textures/managers/animation.json");

	setAnimation(texture, atlas, animation);
	getSprite()->setBatchGroup(fmt::format("room_manager_{}", stage));

	//randomizeProgress();
}

void Factory::Manager::setLevel(int value)
{
	mLevel = value;
	refresh();
}