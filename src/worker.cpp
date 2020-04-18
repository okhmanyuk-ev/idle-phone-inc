#include "worker.h"
#include "helpers.h"

using namespace PhoneInc;

Factory::Worker::Worker(int level) : Scene::MappedAniSprite<WorkerAnimation>(Animations)
{
	setAnimation(TEXTURE("textures/workers/1.png"), ANIMATION("textures/workers/1"));
	setFrequency(12.0f);
	setStateType(Animation::Working);
}