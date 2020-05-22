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

	auto image_path = fmt::format("textures/managers/{}.png", stage);
	auto atlas_path = "textures/managers/atlas.json";
	auto animation_path = "textures/managers/animation.json";
	auto animation = Shared::GraphicsHelpers::OpenAnimationFromFile(image_path, atlas_path, animation_path);
	auto anim_ptr = std::make_shared<Graphics::Animation>(animation);
	auto anim_name = fmt::format("manager_{}", stage);
	CACHE->loadAnimation(anim_ptr, anim_name);
	setAnimation(TEXTURE(image_path), ANIMATION(anim_name));
	//randomizeProgress();
}

void Factory::Manager::setLevel(int value)
{
	mLevel = value;
	refresh();
}