#include "worker.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Factory::Worker::Worker() : Scene::MappedAniSprite<WorkerAnimation>(Animations)
{
	setFrequency(12.0f);
}

void Factory::Worker::refresh()
{
	if (mLevel <= 0)
	{
		setEnabled(false);
		return;
	}

	setEnabled(true);

	auto stage = Balance::GetWorkerStage(mLevel);

	auto image_path = fmt::format("textures/workers/{}.png", stage);
	auto atlas_path = "textures/workers/atlas.json";
	auto animation_path = "textures/workers/animation.json";
	auto animation = std::make_shared<Graphics::Animation>(Graphics::Animation::OpenFromFile(image_path, atlas_path, animation_path));
	auto anim_name = fmt::format("worker_{}", stage);
	CACHE->loadAnimation(animation, anim_name);
	setAnimation(TEXTURE(image_path), ANIMATION(anim_name));
	//randomizeProgress();
}

void Factory::Worker::setLevel(int value)
{
	mLevel = value;
	refresh();
}