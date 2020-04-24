#include "manager.h"
#include "helpers.h"

using namespace PhoneInc;

Factory::Manager::Manager() : Scene::MappedAniSprite<ManagerAnimation>(Animations)
{
	setFrequency(5.0f);
	setStateType(Animation::Work);
}

void Factory::Manager::refresh()
{
	if (mLevel <= 0)
	{
		setEnabled(false);
		return;
	}

	setEnabled(true);

	auto image_path = fmt::format("textures/managers/{}.png", mLevel);
	auto atlas_path = "textures/managers/atlas.json";
	auto animation_path = "textures/managers/animation.json";
	auto animation = Shared::GraphicsHelpers::OpenAnimationFromFile(image_path, atlas_path, animation_path);
	auto anim_ptr = std::make_shared<Graphics::Animation>(animation);
	auto anim_name = fmt::format("manager_{}", mLevel);
	CACHE->loadAnimation(anim_ptr, anim_name);
	setAnimation(TEXTURE(image_path), ANIMATION(anim_name));
	randomizeProgress();
}

void Factory::Manager::setLevel(int value)
{
	mLevel = value;
	refresh();
}