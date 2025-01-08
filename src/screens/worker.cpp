#include "worker.h"
#include "helpers.h"
#include "balance.h"

using namespace PhoneInc;

Factory::Worker::Worker()
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

	auto texture = sky::GetTexture(fmt::format("textures/workers/{}.png", stage));
	auto atlas = sky::GetAtlas("textures/workers/atlas.json");
	auto animation = sky::GetAnimation("textures/workers/animation.json");

	setAnimation(texture, atlas, animation);
	getSprite()->setBatchGroup(fmt::format("room_worker_{}", stage));

	//randomizeProgress();
}

void Factory::Worker::setLevel(int value)
{
	mLevel = value;
	refresh();
}