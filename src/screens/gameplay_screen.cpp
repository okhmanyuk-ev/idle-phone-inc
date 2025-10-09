#include "gameplay_screen.h"
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
	getContent()->attach(mScrollbox);

	auto top_menu = std::make_shared<Scene::AutoSized<Scene::Node>>();
	top_menu->setTouchable(true);
	top_menu->setVerticalOrigin(168.0f);
	top_menu->setAnchor({ 0.5f, 0.0f });
	top_menu->setPivot({ 0.5f, 0.0f });
	getContent()->attach(top_menu);

	auto top_menu_asset = sky::Asset("xml/top_menu.xml");
	auto top_menu_xml = std::string((const char*)top_menu_asset.getMemory(), top_menu_asset.getSize());
	auto [top_menu_node, top_menu_collection] = Shared::SceneHelpers::CreateNodesFromXml(top_menu_xml);
	top_menu->attach(top_menu_node);

	auto cash_label = std::static_pointer_cast<Helpers::LabelSolid>(top_menu_collection.at("cash_label"));
	auto coins_label = std::static_pointer_cast<Helpers::LabelSolid>(top_menu_collection.at("coins_label"));

	runAction(sky::Actions::ExecuteInfinite([coins_label, cash_label](auto dTime) {
		static double cash = 0.0;
		cash = sky::ease_towards(cash, PROFILE->getCash(), dTime);

		static double coins = 0.0;
		coins = sky::ease_towards(coins, PROFILE->getCoins(), dTime);

		cash_label->setText(L"$ " + Helpers::NumberToString(cash));
		coins_label->setText(Helpers::NumberToString(coins));
	}));

	auto bottom_menu_asset = sky::Asset("xml/bottom_menu.xml");
	auto bottom_menu_xml = std::string((const char*)bottom_menu_asset.getMemory(), bottom_menu_asset.getSize());
	auto [bottom_menu_node, bottom_menu_collection] = Shared::SceneHelpers::CreateNodesFromXml(bottom_menu_xml);
	getContent()->attach(bottom_menu_node);

	auto bottom_menu = bottom_menu_collection.at("bottom_menu");

	runAction(sky::Actions::ExecuteInfinite([this, top_menu, bottom_menu] {
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

	runAction(sky::Actions::Delayed(predicate, [microtasks] {
		microtasks->start();
	}));

	runAction(sky::Actions::ExecuteInfinite([this] {
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
	runAction(sky::Actions::ChangePosition(mScrollbox->getContent(), e.pos, 0.5f, Easing::CubicInOut));
}

void GameplayScreen::onEvent(const Helpers::BlockGlobalScrollEvent& e)
{
	mScrollbox->setTouchable(!e.blocked);
}
