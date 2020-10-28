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
	auto name = fmt::format("worker_{}", stage);

	auto load_atlas = [stage] {
		auto texture_path = fmt::format("textures/workers/{}.png", stage);
		auto atlas_path = "textures/workers/atlas.json";
		return std::make_shared<Graphics::Atlas>(TEXTURE(texture_path), Platform::Asset(atlas_path));
	};

	auto load_animation = [name, load_atlas] {
		auto atlas = ATLAS(name, load_atlas);
		auto animation_path = "textures/workers/animation.json";
		return std::make_shared<Graphics::Animation>(atlas, Platform::Asset(animation_path));
	};

	setAnimation(ANIMATION(name, load_animation));

	//randomizeProgress();
}

void Factory::Worker::setLevel(int value)
{
	mLevel = value;
	refresh();
}