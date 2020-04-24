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

	auto texture = TEXTURE("textures/workers/worker.png");
	auto animation = ANIMATION("textures/workers/worker");
	setAnimation(texture, animation);
	randomizeProgress();
}

void Factory::Worker::setLevel(int value)
{
	mLevel = value;
	refresh();
}