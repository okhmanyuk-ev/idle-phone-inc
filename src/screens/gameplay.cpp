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

	auto street = std::make_shared<Street>();
	street->setHorizontalStretch(1.0f);
	
	auto factory = std::make_shared<Factory>();
	factory->setHorizontalStretch(1.0f);
	
	auto width = 1080.0f;

	auto grid = Shared::SceneHelpers::MakeVerticalGrid(width, {
		{ street->getHeight(), street },
		{ factory->getHeight(), factory }
	});

	auto scrollbox = std::make_shared<Scene::Scrollbox>();
	scrollbox->setAnchor({ 0.5f, 0.0f });
	scrollbox->setPivot({ 0.5f, 0.0f });
	scrollbox->setWidth(width);
	scrollbox->getBounding()->setStretch(1.0f);
	scrollbox->setSensitivity({ 0.0f, 1.0f * Helpers::Scale });
	scrollbox->getContent()->setHorizontalStretch(1.0f);
	scrollbox->getContent()->attach(grid);
	scrollbox->getContent()->setHeight(grid->getHeight());
	scrollbox->setTouchMask(1 << 1);
	attach(scrollbox);

	auto top_menu = std::make_shared<TopMenu>();
	top_menu->setAnchor({ 0.5f, 0.0f });
	top_menu->setPivot({ 0.5f, 0.0f });
	attach(top_menu);

	auto bottom_menu = std::make_shared<BottomMenu>();
	bottom_menu->setAnchor({ 0.5f, 1.0f });
	bottom_menu->setPivot({ 0.5f, 1.0f });
	attach(bottom_menu);

	runAction(Shared::ActionHelpers::ExecuteInfinite([this, top_menu, scrollbox, bottom_menu] {
		auto h = getHeight() - scrollbox->getY() - bottom_menu->getHeight();
		scrollbox->setHeight(h);
		scrollbox->setY(top_menu->getHeight());
	}));

	runAction(Shared::ActionHelpers::ExecuteInfinite([] {
		GAME_STATS("warehouse storage", PROFILE->getWarehouseStorage());
	}));
}