#include "gameplay_screen.h"
#include "bottom_menu.h"
#include "top_menu.h"
#include "street.h"
#include "factory.h"
#include "helpers.h"

using namespace PhoneInc;

GameplayScreen::GameplayScreen()
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(sky::GetTexture("textures/background.png"));
	background->setStretch(1.0f);
	getContent()->attach(background);

	auto street = std::make_shared<Street>();
	street->setHorizontalStretch(1.0f);

	auto factory = std::make_shared<Factory>();
	factory->setHorizontalStretch(1.0f);

	auto width = 1080.0f;

	auto column = std::make_shared<Scene::Column>();
	for (auto node : std::vector<std::shared_ptr<Scene::Node>>{ street, factory })
	{
		auto cell = std::make_shared<Scene::Node>();
		cell->setSize({ width, node->getHeight() });
		column->attach(cell);
		cell->attach(node);
	}
	column->setSize({ width, street->getHeight() + factory->getHeight() });

	mScrollbox = std::make_shared<Scene::Scrollbox>();
	mScrollbox->setAnchor({ 0.5f, 0.0f });
	mScrollbox->setPivot({ 0.5f, 0.0f });
	mScrollbox->setWidth(width);
	mScrollbox->getBounding()->setStretch(1.0f);
	mScrollbox->setSensitivity({ 0.0f, 1.0f });
	mScrollbox->getContent()->setHorizontalStretch(1.0f);
	mScrollbox->getContent()->attach(column);
	mScrollbox->getContent()->setHeight(column->getHeight());
	mScrollbox->setTouchMask(1 << 1);
	getContent()->attach(mScrollbox);

	auto top_menu = std::make_shared<TopMenu>();
	top_menu->setAnchor({ 0.5f, 0.0f });
	top_menu->setPivot({ 0.5f, 0.0f });
	getContent()->attach(top_menu);

	auto bottom_menu = std::make_shared<BottomMenu>();
	bottom_menu->setAnchor({ 0.5f, 1.0f });
	bottom_menu->setPivot({ 0.5f, 1.0f });
	getContent()->attach(bottom_menu);

	runAction(Actions::Collection::ExecuteInfinite([this, top_menu, bottom_menu] {
		if (!isTransformReady())
			return;

		auto topSafeMargin = PLATFORM->getSafeAreaTopMargin();
		auto bottomSafeMargin = PLATFORM->getSafeAreaBottomMargin();

		auto unprojectedTopSafeMargin = unproject({ 0.0f, topSafeMargin }).y;
		auto unprojectedBottomSafeMargin = unproject({ 0.0f, bottomSafeMargin }).y;

		top_menu->setY(unprojectedTopSafeMargin);
		bottom_menu->setY(-unprojectedBottomSafeMargin);

		mScrollbox->setY(top_menu->getY() - top_menu->getVerticalOrigin() + top_menu->getHeight());
		mScrollbox->setHeight(getAbsoluteHeight() - mScrollbox->getY() - bottom_menu->getHeight() +
			bottom_menu->getY() - bottom_menu->getVerticalOrigin());
	}));

	// microtasks

	auto microtasks = std::make_shared<MicrotasksHolder>();
	street->attach(microtasks);

	auto predicate = [this] {
		return getState() != State::Entered;
	};

	runAction(Actions::Collection::Delayed(predicate, Actions::Collection::Execute([microtasks] {
		microtasks->start();
	})));

	runAction(Actions::Collection::ExecuteInfinite([this] {
		if (!isTransformReady())
			return;

#if !defined(PLATFORM_MOBILE)
		return;
#endif

		const glm::vec2 target = { 1080.0f, 1920.0f };
		auto size = getAbsoluteSize();
		auto scale = size / target;

		setScale(scale.x);
	}));
}

void GameplayScreen::onEvent(const Helpers::MoveGlobalScrollEvent& e)
{
	runAction(Actions::Collection::ChangePosition(mScrollbox->getContent(), e.pos, 0.5f, Easing::CubicInOut));
}

void GameplayScreen::onEvent(const Helpers::BlockGlobalScrollEvent& e)
{
	mScrollbox->setTouchable(!e.blocked);
}
