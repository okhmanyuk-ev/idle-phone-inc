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

	mProductPanel = std::make_shared<ProductPanel>();
	mProductPanel->setAnchor({ 0.5f, 0.0f });
	mProductPanel->setPivot({ 0.5f, 0.0f });
	mProductPanel->setPosition({ 0.0f, 138.0f });
	background->attach(mProductPanel);

	mManagerPanel = std::make_shared<ManagerPanel>();
	mManagerPanel->setAnchor({ 0.5f, 0.0f });
	mManagerPanel->setPivot({ 0.5f, 0.0f });
	mManagerPanel->setPosition({ 0.0f, 596.0f });
	background->attach(mManagerPanel);

	mWorkerPanel1 = std::make_shared<WorkerPanel>();
	mWorkerPanel1->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel1->setPivot({ 0.5f, 0.0f });
	mWorkerPanel1->setPosition({ 0.0f, 858.0f });
	background->attach(mWorkerPanel1);

	mWorkerPanel2 = std::make_shared<WorkerPanel>();
	mWorkerPanel2->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel2->setPivot({ 0.5f, 0.0f });
	mWorkerPanel2->setPosition({ 0.0f, 1120.0f });
	background->attach(mWorkerPanel2);

	mWorkerPanel3 = std::make_shared<WorkerPanel>();
	mWorkerPanel3->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel3->setPivot({ 0.5f, 0.0f });
	mWorkerPanel3->setPosition({ 0.0f, 1382.0f });
	background->attach(mWorkerPanel3);

	refresh();
	/*
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
	});*/


}

void RoomWindow::refresh()
{
	mProductPanel->refresh();
	mManagerPanel->refresh();
	mWorkerPanel1->refresh();
	mWorkerPanel2->refresh();
	mWorkerPanel3->refresh();
/*	auto& room = PROFILE->getRooms().at(mIndex);

	auto manager_avatar = glm::clamp(room.manager, 1, Profile::Room::MaxManagerLevel);
	auto worker1_avatar = glm::clamp(room.worker1, 1, Profile::Room::MaxWorkerLevel);
	auto worker2_avatar = glm::clamp(room.worker2, 1, Profile::Room::MaxWorkerLevel);
	auto worker3_avatar = glm::clamp(room.worker3, 1, Profile::Room::MaxWorkerLevel);

	mProduct.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/products/{}.png", room.product)));
	mManager.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/managers/{}.png", manager_avatar)));
	mWorker1.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", worker1_avatar)));
	mWorker2.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", worker2_avatar)));
	mWorker3.icon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", worker3_avatar)));

	auto hasProduct = room.product > 0;
	auto hasManager = room.manager > 0;
	auto hasWorker1 = room.worker1 > 0;
	auto hasWorker2 = room.worker2 > 0;
	auto hasWorker3 = room.worker3 > 0;

	auto hasWorkers = hasWorker1 || hasWorker2 || hasWorker3;

	mManager.button->setActive(hasWorkers);

	mWorker1.button->setActive(hasProduct);
	mWorker2.button->setActive(hasProduct);
	mWorker3.button->setActive(hasProduct);

	mProduct.button->setEnabled(room.product < Profile::Room::MaxProductLevel);
	mManager.button->setEnabled(room.manager < Profile::Room::MaxManagerLevel);
	mWorker1.button->setEnabled(room.worker1 < Profile::Room::MaxWorkerLevel);
	mWorker2.button->setEnabled(room.worker2 < Profile::Room::MaxWorkerLevel);
	mWorker3.button->setEnabled(room.worker3 < Profile::Room::MaxWorkerLevel);

	auto unlock_text = LOCALIZE("UNLOCK_BUTTON");
	auto upgrade_text = LOCALIZE("UPGRADE_BUTTON");

	mProduct.button->getLabel()->setText(hasProduct ? upgrade_text : unlock_text);
	mManager.button->getLabel()->setText(hasManager ? upgrade_text : unlock_text);
	mWorker1.button->getLabel()->setText(hasWorker1 ? upgrade_text : unlock_text);
	mWorker2.button->getLabel()->setText(hasWorker2 ? upgrade_text : unlock_text);
	mWorker3.button->getLabel()->setText(hasWorker3 ? upgrade_text : unlock_text);

	mManager.title->setText(hasManager ? LOCALIZE("ROOM_WINDOW_MANAGER_TITLE") : LOCALIZE("ROOM_WINDOW_MANAGER_TITLE_HIRE"));
	mWorker1.title->setText(hasWorker1 ? LOCALIZE_FMT("ROOM_WINDOW_WORKER_TITLE", 1) : LOCALIZE("ROOM_WINDOW_WORKER_TITLE_HIRE"));
	mWorker2.title->setText(hasWorker2 ? LOCALIZE_FMT("ROOM_WINDOW_WORKER_TITLE", 2) : LOCALIZE("ROOM_WINDOW_WORKER_TITLE_HIRE"));
	mWorker3.title->setText(hasWorker3 ? LOCALIZE_FMT("ROOM_WINDOW_WORKER_TITLE", 3) : LOCALIZE("ROOM_WINDOW_WORKER_TITLE_HIRE"));

	mManager.description->setText(LOCALIZE_FMT("ROOM_WINDOW_LEVEL_DESCRIPTION", room.manager));
	mWorker1.description->setText(LOCALIZE_FMT("ROOM_WINDOW_LEVEL_DESCRIPTION", room.worker1));
	mWorker2.description->setText(LOCALIZE_FMT("ROOM_WINDOW_LEVEL_DESCRIPTION", room.worker2));
	mWorker3.description->setText(LOCALIZE_FMT("ROOM_WINDOW_LEVEL_DESCRIPTION", room.worker3));*/
}

void RoomWindow::event(const Profile::RoomChangedEvent& e)
{
	if (e.index != mIndex)
		return;

	refresh();
}

/*std::shared_ptr<Scene::Node> RoomWindow::createMajorPanel(MajorPanel& panel)
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/windows/room_window/major_panel_backgorund.png"));

	auto icon = std::make_shared<Helpers::Adaptive<Scene::Sprite>>();
	icon->setAnchor({ 0.0f, 0.5f });
	icon->setPivot({ 0.5f, 0.5f });
	icon->setPosition({ 124.0f, 0.0f });
	icon->setAdaptSize({ 174.0f, 298.0f });
	background->attach(icon);

	auto button = std::make_shared<Helpers::StandardButton>();
	button->setAnchor({ 1.0f, 0.5f });
	button->setPivot({ 1.0f, 0.5f });
	button->setPosition({ -28.0f, 0.0f });
	background->attach(button);

	panel.icon = icon;
	panel.button = button;

	return background;
}*/

/*std::shared_ptr<Scene::Node> RoomWindow::createMinorPanel(MinorPanel_s& panel)
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/windows/room_window/minor_panel_backgorund.png"));

	auto icon = std::make_shared<Scene::Sprite>();
	icon->setAnchor({ 0.0f, 0.5f });
	icon->setPivot({ 0.5f, 0.5f });
	icon->setPosition({ 124.0f, 0.0f });
	background->attach(icon);

	auto title = std::make_shared<Helpers::LabelSolid>();
	title->setFontSize(12.0f);
	title->setAnchor({ 0.0f, 0.0f });
	title->setPivot({ 0.0f, 0.5f });
	title->setPosition({ 264.0f, 64.0f });
	title->setText("worker1");
	title->setColor(Graphics::Color::ToNormalized(12, 22, 44));
	background->attach(title);

	auto description = std::make_shared<Helpers::LabelSolid>();
	description->setFontSize(12.0f);
	description->setAnchor({ 0.0f, 0.0f });
	description->setPivot({ 0.0f, 0.5f });
	description->setPosition({ 264.0f, 114.0f });
	description->setText("level 1");
	description->setColor(Graphics::Color::ToNormalized(23, 0, 164));
	background->attach(description);

	auto button = std::make_shared<Helpers::StandardButton>();
	button->setAnchor({ 1.0f, 0.5f });
	button->setPivot({ 1.0f, 0.5f });
	button->setPosition({ -28.0f, 0.0f });
	button->getLabel()->setText(LOCALIZE("UNLOCK_BUTTON"));
	background->attach(button);

	panel.icon = icon;
	panel.title = title;
	panel.description = description;
	panel.button = button;

	return background;
}*/

// product panel

RoomWindow::ProductPanel::ProductPanel()
{
	setTexture(TEXTURE("textures/windows/room_window/major_panel_backgorund.png"));
}

void RoomWindow::ProductPanel::refresh()
{
	//
}

// small panel

RoomWindow::SmallPanel::SmallPanel()
{
	setTexture(TEXTURE("textures/windows/room_window/minor_panel_backgorund.png"));

	mIcon = std::make_shared<Scene::Sprite>();
	mIcon->setAnchor({ 0.0f, 0.5f });
	mIcon->setPivot({ 0.5f, 0.5f });
	mIcon->setPosition({ 124.0f, 0.0f });
	attach(mIcon);
 
	auto button = std::make_shared<Helpers::StandardButton>();
	button->setAnchor({ 1.0f, 0.5f });
	button->setPivot({ 1.0f, 0.5f });
	button->setPosition({ -28.0f, 0.0f });
	attach(button);
}

void RoomWindow::SmallPanel::refresh()
{
	mIcon->setTexture(getIconTexture());
}

// manager panel

std::shared_ptr<Renderer::Texture> RoomWindow::ManagerPanel::getIconTexture() const
{
	return TEXTURE(fmt::format("textures/windows/room_window/avatars/managers/{}.png", 1));
}

// worker panel

std::shared_ptr<Renderer::Texture> RoomWindow::WorkerPanel::getIconTexture() const
{
	return TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", 1));
}
