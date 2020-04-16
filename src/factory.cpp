#include "factory.h"
#include "helpers.h"

using namespace PhoneInc;

Factory::Factory()
{
	setHeight(300.0f);

	auto conveyor_hat = std::make_shared<Scene::Sprite>();
	conveyor_hat->setTexture(TEXTURE("textures/factory/conveyor_hat.png"));
	attach(conveyor_hat);

	enum class Animation
	{
		Work,
		Idle
	};
	
	const Scene::MappedAniSprite<Animation>::StateMap Animations = {
		{ Animation::Work, "work" },
		{ Animation::Idle, "idle" }
	};

	auto worker = std::make_shared<Scene::MappedAniSprite<Animation>>(Animations);
	worker->setAnimation(std::make_shared<Graphics::Animation>(Shared::GraphicsHelpers::OpenAnimationFromFile("textures/workers/1")));
	worker->setSize(168.0f);
	worker->setAnchor(0.5f);
	worker->setPivot(0.5f);
	worker->setFrequency(5.0f);
	worker->setStateType(Animation::Work);
	worker->setScale(Helpers::Scale);
	attach(worker);
}