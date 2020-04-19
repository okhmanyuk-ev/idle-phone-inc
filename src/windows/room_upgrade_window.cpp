#include "room_upgrade_window.h"

using namespace PhoneInc;

RoomUpgradeWindow::RoomUpgradeWindow()
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/windows/room_upgrade_window/background.png"));
	background->setAnchor(0.5f);
	background->setPivot(0.5f);
	background->setTouchable(true);
	getContent()->attach(background);

	auto title = std::make_shared<Helpers::LabelSolid>();
	title->setAnchor({ 0.5f, 0.0f });
	title->setPivot(0.5f);
	title->setFontSize(15.0f);
	title->setPosition({ 0.0f, 64.0f });
	title->setText(LOCALIZE("ROOM_UPGRADE_WINDOW_TITLE"));
	background->attach(title);

	auto close = std::make_shared<Helpers::CloseButtonWidget>();
	close->setPivot(0.5f);
	close->setAnchor({ 1.0f, 0.0f });
	close->setPosition({ -72.0f, 62.0f });
	background->attach(close);

	auto choose_x = std::make_shared<Helpers::ChooseMultiplierWidget>();
	choose_x->setAnchor({ 0.5f, 1.0f });
	choose_x->setPivot({ 0.5f, 1.0f });
	choose_x->setPosition({ 0.0f, -26.0f });
	background->attach(choose_x);

}