#include "room_window.h"

using namespace PhoneInc;

RoomWindow::RoomWindow(int index)
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/windows/room_window/background.png"));
	background->setAnchor(0.5f);
	background->setPivot(0.5f);
	background->setTouchable(true);
	getContent()->attach(background);

	auto title = std::make_shared<Helpers::LabelSolid>();
	title->setAnchor({ 0.5f, 0.0f });
	title->setPivot(0.5f);
	title->setFontSize(15.0f);
	title->setPosition({ 0.0f, 64.0f });
	title->setText(LOCALIZE_FMT("ROOM_WINDOW_TITLE", index + 1));
	background->attach(title);

	auto close = std::make_shared<Helpers::CloseButtonWidget>();
	close->setPivot(0.5f);
	close->setAnchor({ 1.0f, 0.0f });
	close->setPosition({ -72.0f, 62.0f });
	background->attach(close);

	auto major_panel = createMajorPanel();
	major_panel->setAnchor({ 0.5f, 0.0f });
	major_panel->setPivot({ 0.5f, 0.0f });
	major_panel->setPosition({ 0.0f, 138.0f });
	background->attach(major_panel);

	auto manager_panel = createMinorPanel();
	manager_panel->setAnchor({ 0.5f, 0.0f });
	manager_panel->setPivot({ 0.5f, 0.0f });
	manager_panel->setPosition({ 0.0f, 596.0f });
	background->attach(manager_panel);

	auto worker_1_panel = createMinorPanel();
	worker_1_panel->setAnchor({ 0.5f, 0.0f });
	worker_1_panel->setPivot({ 0.5f, 0.0f });
	worker_1_panel->setPosition({ 0.0f, 858.0f });
	background->attach(worker_1_panel);

	auto worker_2_panel = createMinorPanel();
	worker_2_panel->setAnchor({ 0.5f, 0.0f });
	worker_2_panel->setPivot({ 0.5f, 0.0f });
	worker_2_panel->setPosition({ 0.0f, 1120.0f });
	background->attach(worker_2_panel);

	auto worker_3_panel = createMinorPanel();
	worker_3_panel->setAnchor({ 0.5f, 0.0f });
	worker_3_panel->setPivot({ 0.5f, 0.0f });
	worker_3_panel->setPosition({ 0.0f, 1382.0f });
	background->attach(worker_3_panel);
}

std::shared_ptr<Scene::Node> RoomWindow::createMajorPanel()
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/windows/room_window/major_panel_backgorund.png"));

	auto button = std::make_shared<Helpers::StandardButton>();
	button->setAnchor({ 1.0f, 0.5f });
	button->setPivot({ 1.0f, 0.5f });
	button->setPosition({ -28.0f, 0.0f });
	button->getLabel()->setText(LOCALIZE("UNLOCK_BUTTON"));
	background->attach(button);

	return background;
}

std::shared_ptr<Scene::Node> RoomWindow::createMinorPanel()
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/windows/room_window/minor_panel_backgorund.png"));

	auto icon = std::make_shared<Scene::Sprite>();
	icon->setTexture(TEXTURE("textures/windows/room_window/avatars/workers/1.png"));
	icon->setAnchor({ 0.0f, 0.5f });
	icon->setPivot({ 0.0f, 0.5f });
	icon->setPosition({ 32.0f, 0.0f });
	background->attach(icon);

	auto button = std::make_shared<Helpers::StandardButton>();
	button->setAnchor({ 1.0f, 0.5f });
	button->setPivot({ 1.0f, 0.5f });
	button->setPosition({ -28.0f, 0.0f });
	button->getLabel()->setText(LOCALIZE("UNLOCK_BUTTON"));
	background->attach(button);

	return background;
}
