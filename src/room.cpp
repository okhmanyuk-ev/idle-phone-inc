#include "room.h"
#include "worker.h"
#include "manager.h"

using namespace PhoneInc;

Factory::Room::Room(int number)
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
	worker1->randomizeProgress();
	table->attach(worker1);
	
	auto worker2 = std::make_shared<Worker>(2);
	worker2->setAnchor({ 0.5f, 0.0f });
	worker2->setPivot({ 0.5f, 1.0f });
	worker2->setY(24.0f);
	worker2->randomizeProgress();
	table->attach(worker2);
	
	auto worker3 = std::make_shared<Worker>(3);
	worker3->setAnchor({ 0.8f, 0.0f });
	worker3->setPivot({ 0.5f, 1.0f });
	worker3->setY(24.0f);
	worker3->randomizeProgress();
	table->attach(worker3);

	auto manager = std::make_shared<Manager>(1);
	manager->setPosition({ -18.0f, 116.0f });
	manager->randomizeProgress();
	attach(manager);

	auto lvl_label = std::make_shared<Helpers::LabelSolid>();
	lvl_label->setFont(FONT("default_bold"));
	lvl_label->setPosition({ 50.0f, 59.0f });
	lvl_label->setPivot(0.5f);
	lvl_label->setText(std::to_string(number));
	lvl_label->setColor(Graphics::Color::Black);
	lvl_label->setFontSize(11.0f);
	attach(lvl_label);

	auto enhance_btn = std::make_shared<Helpers::StandardButton>();
	enhance_btn->setPosition({ 681.0f, 28.0f });
	enhance_btn->getLabel()->setText(LOCALIZE("ENHANCE_BUTTON"));
	attach(enhance_btn);
}

Factory::LockedRoom::LockedRoom(int number)
{
	setTexture(TEXTURE("textures/factory/room/background/locked.png"));

	auto button = std::make_shared<Helpers::StandardLongButton>();
	button->getLabel()->setText("awdawd");
	button->setAnchor(0.5f);
	button->setPivot(0.5f);
	button->setClickCallback([this] {
		mUnlockCallback();
	});
	attach(button);
}