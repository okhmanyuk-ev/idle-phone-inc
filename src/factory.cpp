#include "factory.h"
#include "helpers.h"

using namespace PhoneInc;

Factory::Factory()
{
	setHeight(300.0f);

	auto conveyor_hat = std::make_shared<Scene::Sprite>();
	conveyor_hat->setTexture(TEXTURE("textures/factory/conveyor_hat.png"));
	attach(conveyor_hat);
}