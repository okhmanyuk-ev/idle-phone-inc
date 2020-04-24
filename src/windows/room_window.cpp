#include "room_window.h"

using namespace PhoneInc;

RoomWindow::RoomWindow(int index) : mIndex(index)
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

	auto major_panel = createMajorPanel(mProduct);
	major_panel->setAnchor({ 0.5f, 0.0f });
	major_panel->setPivot({ 0.5f, 0.0f });
	major_panel->setPosition({ 0.0f, 138.0f });
	background->attach(major_panel);

	auto manager_panel = createMinorPanel(mManager);
	manager_panel->setAnchor({ 0.5f, 0.0f });
	manager_panel->setPivot({ 0.5f, 0.0f });
	manager_panel->setPosition({ 0.0f, 596.0f });
	background->attach(manager_panel);

	auto worker_1_panel = createMinorPanel(mWorker1);
	worker_1_panel->setAnchor({ 0.5f, 0.0f });
	worker_1_panel->setPivot({ 0.5f, 0.0f });
	worker_1_panel->setPosition({ 0.0f, 858.0f });
	background->attach(worker_1_panel);

	auto worker_2_panel = createMinorPanel(mWorker2);
	worker_2_panel->setAnchor({ 0.5f, 0.0f });
	worker_2_panel->setPivot({ 0.5f, 0.0f });
	worker_2_panel->setPosition({ 0.0f, 1120.0f });
	background->attach(worker_2_panel);

	auto worker_3_panel = createMinorPanel(mWorker3);
	worker_3_panel->setAnchor({ 0.5f, 0.0f });
	worker_3_panel->setPivot({ 0.5f, 0.0f });
	worker_3_panel->setPosition({ 0.0f, 1382.0f });
	background->attach(worker_3_panel);

	refresh();

	mProduct.button->setActiveCallback([this] {
		auto room = PROFILE->getRooms().at(mIndex);
		room.product += 1;
		PROFILE->setRoom(mIndex, room);
	});
	mManager.button->setActiveCallback([this] {
		auto room = PROFILE->getRooms().at(mIndex);
		room.manager += 1;
		PROFILE->setRoom(mIndex, room);
	});
	mWorker1.button->setActiveCallback([this] {
		auto room = PROFILE->getRooms().at(mIndex);
		room.worker1 += 1;
		PROFILE->setRoom(mIndex, room);
	});
	mWorker2.button->setActiveCallback([this] {
		auto room = PROFILE->getRooms().at(mIndex);
		room.worker2 += 1;
		PROFILE->setRoom(mIndex, room);
	});
	mWorker3.button->setActiveCallback([this] {
		auto room = PROFILE->getRooms().at(mIndex);
		room.worker3 += 1;
		PROFILE->setRoom(mIndex, room);
	});
}

void RoomWindow::refresh()
{
	auto& room = PROFILE->getRooms().at(mIndex);

	auto manager_avatar = glm::clamp(room.manager, 1, Profile::Room::MaxManagerLevel);
	auto worker1_avatar = glm::clamp(room.worker1, 1, Profile::Room::MaxWorkerLevel);
	auto worker2_avatar = glm::clamp(room.worker2, 1, Profile::Room::MaxWorkerLevel);
	auto worker3_avatar = glm::clamp(room.worker3, 1, Profile::Room::MaxWorkerLevel);

	mProduct.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/products/{}.png", room.product)));
	mManager.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/managers/{}.png", manager_avatar)));
	mWorker1.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", worker1_avatar)));
	mWorker2.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", worker2_avatar)));
	mWorker3.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", worker3_avatar)));

	auto hasWorkers = room.worker1 > 0 || room.worker2 > 0 || room.worker3 > 0;
	mManager.button->setActive(hasWorkers);

	auto hasProduct = room.product > 0;

	mWorker1.button->setActive(hasProduct);
	mWorker2.button->setActive(hasProduct);
	mWorker3.button->setActive(hasProduct);
}

void RoomWindow::event(const Profile::RoomChangedEvent& e)
{
	if (e.index != mIndex)
		return;

	refresh();
}

std::shared_ptr<Scene::Node> RoomWindow::createMajorPanel(MajorPanel& panel)
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/windows/room_window/major_panel_backgorund.png"));

	auto icon = std::make_shared<Scene::Sprite>();
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

	panel.icon = icon;
	panel.button = button;

	return background;
}

std::shared_ptr<Scene::Node> RoomWindow::createMinorPanel(MinorPanel& panel)
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/windows/room_window/minor_panel_backgorund.png"));

	auto icon = std::make_shared<Scene::Sprite>();
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

	panel.icon = icon;
	panel.button = button;

	return background;
}
