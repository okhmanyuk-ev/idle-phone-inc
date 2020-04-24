#include "worker.h"
#include "helpers.h"

using namespace PhoneInc;

Factory::Worker::Worker() : Scene::MappedAniSprite<WorkerAnimation>(Animations)
{
	setFrequency(12.0f);
	setStateType(Animation::Working);
}

void Factory::Worker::refresh()
{
	if (mLevel <= 0)
	{
		setEnabled(false);
		return;
	}

	setEnabled(true);

	auto image_path = fmt::format("textures/workers/{}.png", mLevel);
	auto atlas_path = "textures/workers/atlas.json";
	auto animation_path = "textures/workers/animation.json";
	auto animation = Shared::GraphicsHelpers::OpenAnimationFromFile(image_path, atlas_path, animation_path);
	auto anim_ptr = std::make_shared<Graphics::Animation>(animation);
	auto anim_name = fmt::format("worker_{}", mLevel);
	CACHE->loadAnimation(anim_ptr, anim_name);
	setAnimation(TEXTURE(image_path), ANIMATION(anim_name));
	randomizeProgress();
}

void Factory::Worker::setLevel(int value)
{
	mLevel = value;
	refresh();
}