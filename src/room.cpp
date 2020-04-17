#include "room.h"
#include "helpers.h"
#include "worker.h"
#include "manager.h"

using namespace PhoneInc;

Factory::Room::Room()
{
	setTexture(TEXTURE("textures/factory/room/background/1.png"));

	auto table = std::make_shared<Scene::Sprite>();
	table->setTexture(TEXTURE("textures/factory/room/table/1.png"));
	table->setAnchor({ 0.5f, 1.0f });
	table->setPivot({ 0.5f, 1.0f });
	table->setY(-56.0f);
	attach(table);

	auto worker1 = std::make_shared<Worker>(1);
	worker1->setAnchor({ 0.2f, 0.0f });
	worker1->setPivot({ 0.5f, 1.0f });
	worker1->setY(24.0f);
	table->attach(worker1);

	auto worker2 = std::make_shared<Worker>(2);
	worker2->setAnchor({ 0.5f, 0.0f });
	worker2->setPivot({ 0.5f, 1.0f });
	worker2->setY(24.0f);
	table->attach(worker2);

	auto worker3 = std::make_shared<Worker>(3);
	worker3->setAnchor({ 0.8f, 0.0f });
	worker3->setPivot({ 0.5f, 1.0f });
	worker3->setY(24.0f);
	table->attach(worker3);

	auto manager = std::make_shared<Manager>(1);
	manager->setPosition({ -18.0f, 116.0f });
	attach(manager);
}