#include "manager.h"
#include "helpers.h"

using namespace PhoneInc;

Factory::Manager::Manager(int level) : Scene::MappedAniSprite<ManagerAnimation>(Animations)
{
	auto image_path = "textures/managers/" + std::to_string(level) + ".png";
	auto atlas_path = "textures/managers/atlas.json";
	auto animation_path = "textures/managers/animation.json";
	auto animation = Shared::GraphicsHelpers::OpenAnimationFromFile(image_path, atlas_path, animation_path);
	setAnimation(std::make_shared<Graphics::Animation>(animation));
	setFrequency(5.0f);
	setStateType(ManagerAnimation::Work);
}