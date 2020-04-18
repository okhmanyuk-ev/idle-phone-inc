#include "manager.h"
#include "helpers.h"

using namespace PhoneInc;

Factory::Manager::Manager(int level) : Scene::MappedAniSprite<ManagerAnimation>(Animations)
{
	auto image_path = "textures/managers/" + std::to_string(level) + ".png";
	auto atlas_path = "textures/managers/atlas.json";
	auto animation_path = "textures/managers/animation.json";
	auto animation = Shared::GraphicsHelpers::OpenAnimationFromFile(image_path, atlas_path, animation_path);
	auto anim_ptr = std::make_shared<Graphics::Animation>(animation);
	auto anim_name = "manager_" + std::to_string(level);
	CACHE->loadAnimation(anim_ptr, anim_name);
	setAnimation(TEXTURE(image_path), ANIMATION(anim_name));
	setFrequency(5.0f);
	setStateType(Animation::Work);
}