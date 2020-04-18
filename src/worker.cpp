#include "worker.h"
#include "helpers.h"

using namespace PhoneInc;

Factory::Worker::Worker(int level) : Scene::MappedAniSprite<WorkerAnimation>(Animations)
{
	auto image_path = "textures/workers/1.png";
	auto atlas_path = "textures/workers/atlas.json";
	auto animation_path = "textures/workers/animation.json";
	auto animation = Shared::GraphicsHelpers::OpenAnimationFromFile(image_path, atlas_path, animation_path);
	setAnimation(std::make_shared<Graphics::Animation>(animation));
	setFrequency(12.0f);
	setStateType(Animation::Working);
}