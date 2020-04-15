#include "gameplay.h"
#include "bottom_menu.h"
#include "top_menu.h"
#include "street.h"
#include "factory.h"
#include "helpers.h"

using namespace PhoneInc;

Gameplay::Gameplay()
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/background.png"));
	background->setStretch(1.0f);
	attach(background);

	auto factory = std::make_shared<Factory>();
	factory->setAnchor({ 0.5f, 0.0f });
	factory->setPivot({ 0.5f, 0.0f });
	factory->setScale(Helpers::InvScale);
	factory->setWidth(1080.0f);
	factory->setY(264.0f);
	attach(factory);

	auto street = std::make_shared<Street>();
	street->setAnchor({ 0.5f, 0.0f });
	street->setPivot({ 0.5f, 0.0f });
	street->setY(37.0f);
	street->setScale(Helpers::InvScale);
	attach(street);

	auto top_menu = std::make_shared<TopMenu>();
	top_menu->setAnchor({ 0.5f, 0.0f });
	top_menu->setPivot({ 0.5f, 0.0f });
	top_menu->setScale(Helpers::InvScale);
	attach(top_menu);

	auto bottom_menu = std::make_shared<BottomMenu>();
	bottom_menu->setAnchor({ 0.5f, 1.0f });
	bottom_menu->setPivot({ 0.5f, 1.0f });
	bottom_menu->setScale(Helpers::InvScale);
	attach(bottom_menu);

	runAction(Shared::ActionHelpers::ExecuteInfinite([this, factory, bottom_menu] {
		auto h = getHeight() - factory->getY() - (bottom_menu->getHeight() / Helpers::Scale);
		factory->setHeight(h * Helpers::Scale);
	}));
}