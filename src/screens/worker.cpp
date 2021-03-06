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
	
	auto texture = TEXTURE(fmt::format("textures/workers/{}.png", stage));
	auto atlas = ATLAS("textures/workers/atlas.json");
	auto animation = ANIMATION("textures/workers/animation.json");

	setAnimation(texture, atlas, animation);

	//randomizeProgress();
}

void Factory::Worker::setLevel(int value)
{
	mLevel = value;
	refresh();
}