#include "factory.h"
#include "helpers.h"
#include "room.h"

using namespace PhoneInc;

Factory::Factory()
{
	setHeight(300.0f);

	auto room = std::make_shared<Room>();
	room->setAnchor({ 1.0f, 0.0f });
	room->setPivot({ 1.0f, 0.0f });
	attach(room);

	auto conveyor_hat = std::make_shared<Scene::Sprite>();
	conveyor_hat->setTexture(TEXTURE("textures/factory/conveyor_hat.png"));
	attach(conveyor_hat);
}