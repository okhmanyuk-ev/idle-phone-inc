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
	street->setScale(Helpers::InvScale);
	
	auto factory = std::make_shared<Factory>();
	factory->setHorizontalStretch(1.0f);
	factory->setScale(Helpers::InvScale);
	
	auto width = 1080.0f;

	auto grid = Shared::SceneHelpers::MakeVerticalGrid(width, {
		{ street->getHeight() / Helpers::Scale, street },
		{ factory->getHeight() / Helpers::Scale, factory }
	});

	auto scrollbox = std::make_shared<Scene::Scrollbox>();
	scrollbox->setAnchor({ 0.5f, 0.0f });
	scrollbox->setPivot({ 0.5f, 0.0f });
	scrollbox->setWidth(width / Helpers::Scale);
	scrollbox->setY(37.0f);
	scrollbox->getBounding()->setStretch(1.0f);
	scrollbox->setSensitivity({ 0.0f, 1.0f });
	scrollbox->getContent()->setHorizontalStretch(1.0f);
	scrollbox->getContent()->attach(grid);
	scrollbox->getContent()->setHeight(grid->getHeight());
	scrollbox->setTouchMask(1 << 1);
	attach(scrollbox);

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

	runAction(Shared::ActionHelpers::ExecuteInfinite([this, scrollbox, bottom_menu] {
		auto h = getHeight() - scrollbox->getY() - (bottom_menu->getHeight() / Helpers::Scale);
		scrollbox->setHeight(h);
	}));

	auto region = Graphics::TexRegion();
	region.pos = { 7.0f, 7.0f };
	region.size = { 3.0f, 3.0f };

	auto test = std::make_shared<Scene::SlicedSprite>();
	test->setTexture(TEXTURE("textures/ui/progressbar_background.png"));
	test->setAnchor(0.5f);
	test->setPivot(0.5f);
	test->setSize(128);
	test->setCenterRegion(region);
	attach(test);
}