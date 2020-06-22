#include "gameplay_screen.h"
#include "bottom_menu.h"
#include "top_menu.h"
#include "street.h"
#include "factory.h"
#include "helpers.h"

using namespace PhoneInc;

GameplayScreen::GameplayScreen()
{
	auto root = getContent();

	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/background.png"));
	background->setStretch(1.0f);
	root->attach(background);

	auto street = std::make_shared<Street>();
	street->setHorizontalStretch(1.0f);
	
	auto factory = std::make_shared<Factory>();
	factory->setHorizontalStretch(1.0f);
	
	auto width = 1080.0f;

	auto grid = Shared::SceneHelpers::MakeVerticalGrid(width, {
		{ street->getHeight(), street },
		{ factory->getHeight(), factory }
	});

	mScrollbox = std::make_shared<Scene::Scrollbox>();
	mScrollbox->setAnchor({ 0.5f, 0.0f });
	mScrollbox->setPivot({ 0.5f, 0.0f });
	mScrollbox->setWidth(width);
	mScrollbox->getBounding()->setStretch(1.0f);
	mScrollbox->setSensitivity({ 0.0f, 1.0f });
	mScrollbox->getContent()->setHorizontalStretch(1.0f);
	mScrollbox->getContent()->attach(grid);
	mScrollbox->getContent()->setHeight(grid->getHeight());
	mScrollbox->setTouchMask(1 << 1);
	root->attach(mScrollbox);

	auto top_menu = std::make_shared<TopMenu>();
	top_menu->setAnchor({ 0.5f, 0.0f });
	top_menu->setPivot({ 0.5f, 0.0f });
	root->attach(top_menu);

	auto bottom_menu = std::make_shared<BottomMenu>();
	bottom_menu->setAnchor({ 0.5f, 1.0f });
	bottom_menu->setPivot({ 0.5f, 1.0f });
	root->attach(bottom_menu);

	runAction(Shared::ActionHelpers::ExecuteInfinite([this, top_menu, bottom_menu] {
		auto h = getHeight() - mScrollbox->getY() - bottom_menu->getHeight();
		mScrollbox->setHeight(h);
		mScrollbox->setY(top_menu->getHeight());
	}));
}

void GameplayScreen::event(const Helpers::MoveGlobalScrollEvent& e)
{
	runAction(Shared::ActionHelpers::ChangePosition(mScrollbox->getContent(), e.pos, 0.5f, Common::Easing::CubicInOut));
}

void GameplayScreen::event(const Helpers::BlockGlobalScrollEvent& e)
{
	mScrollbox->setTouchable(!e.blocked);
}
