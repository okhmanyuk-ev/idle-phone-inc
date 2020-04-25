#include "room_window.h"
#include "balance.h"

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

	mProductPanel = std::make_shared<ProductPanel>(index);
	mProductPanel->setAnchor({ 0.5f, 0.0f });
	mProductPanel->setPivot({ 0.5f, 0.0f });
	mProductPanel->setPosition({ 0.0f, 138.0f });
	background->attach(mProductPanel);

	mManagerPanel = std::make_shared<ManagerPanel>(index);
	mManagerPanel->setAnchor({ 0.5f, 0.0f });
	mManagerPanel->setPivot({ 0.5f, 0.0f });
	mManagerPanel->setPosition({ 0.0f, 596.0f });
	background->attach(mManagerPanel);

	mWorkerPanel1 = std::make_shared<WorkerPanel>(index, 1);
	mWorkerPanel1->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel1->setPivot({ 0.5f, 0.0f });
	mWorkerPanel1->setPosition({ 0.0f, 858.0f });
	background->attach(mWorkerPanel1);

	mWorkerPanel2 = std::make_shared<WorkerPanel>(index, 2);
	mWorkerPanel2->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel2->setPivot({ 0.5f, 0.0f });
	mWorkerPanel2->setPosition({ 0.0f, 1120.0f });
	background->attach(mWorkerPanel2);

	mWorkerPanel3 = std::make_shared<WorkerPanel>(index, 3);
	mWorkerPanel3->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel3->setPivot({ 0.5f, 0.0f });
	mWorkerPanel3->setPosition({ 0.0f, 1382.0f });
	background->attach(mWorkerPanel3);

	refresh();
}

void RoomWindow::refresh()
{
	mProductPanel->refresh();
	mManagerPanel->refresh();
	mWorkerPanel1->refresh();
	mWorkerPanel2->refresh();
	mWorkerPanel3->refresh();
}

void RoomWindow::event(const Profile::RoomChangedEvent& e)
{
	if (e.index != mIndex)
		return;

	refresh();
}

void RoomWindow::event(const Profile::CashChangedEvent& e)
{
	refresh();
}

// panel

RoomWindow::Panel::Panel(int roomIndex) : mRoomIndex(roomIndex)
{
	mButton = std::make_shared<Helpers::StandardButton>();
	mButton->setAnchor({ 1.0f, 0.5f });
	mButton->setPivot({ 1.0f, 0.5f });
	mButton->setPosition({ -28.0f, 0.0f });
	mButton->setActiveCallback([this] {
		PROFILE->spendCash(getUpgradeCost());
		increaseLevel();
	});
	attach(mButton);

	mButton->getLabel()->setY(12.0f);
	mButton->getLabel()->setFontSize(12.0f);

	mButtonAdditionalLabel = std::make_shared<Helpers::Label>();
	mButtonAdditionalLabel->setPivot(0.5f);
	mButtonAdditionalLabel->setAnchor(0.5f);
	mButtonAdditionalLabel->setFontSize(12.0f);
	mButtonAdditionalLabel->setY(-34.0f);
	mButton->attach(mButtonAdditionalLabel);
}

void RoomWindow::Panel::refresh()
{
	mButton->setEnabled(!isLastLevel());

	if (mButton->isEnabled())
	{
		if (isOpened())
		{
			mButton->setActive(PROFILE->isEnoughCash(getUpgradeCost()));
			mButtonAdditionalLabel->setText(getUpgradeButtonText());
		}
		else
		{
			if (isOpenAvailable())
			{
				mButton->setActive(PROFILE->isEnoughCash(getUpgradeCost()));
			}
			else
			{
				mButton->setActive(false);
			}			
			mButtonAdditionalLabel->setText(getOpenButtonText());
		}
		mButton->getLabel()->setText(Helpers::NumberToString(getUpgradeCost()));
	}
}

utf8_string RoomWindow::Panel::getLevelText() const
{
	return LOCALIZE_FMT("ROOM_WINDOW_LEVEL_DESCRIPTION", getLevel());
}

// product panel

RoomWindow::ProductPanel::ProductPanel(int roomIndex) : Panel(roomIndex)
{
	setTexture(TEXTURE("textures/windows/room_window/major_panel_backgorund.png"));

	mIcon = std::make_shared<Helpers::Adaptive<Scene::Sprite>>();
	mIcon->setAnchor({ 0.0f, 0.5f });
	mIcon->setPivot({ 0.5f, 0.5f });
	mIcon->setPosition({ 124.0f, 0.0f });
	mIcon->setAdaptSize({ 174.0f, 298.0f });
	attach(mIcon);

	mLevelLabel = std::make_shared<Helpers::LabelSolid>();
	mLevelLabel->setFontSize(12.0f);
	mLevelLabel->setAnchor({ 0.0f, 0.0f });
	mLevelLabel->setPivot({ 0.0f, 0.5f });
	mLevelLabel->setPosition({ 264.0f, 222.0f });
	mLevelLabel->setColor(Graphics::Color::ToNormalized(23, 0, 164));
	attach(mLevelLabel);
}

void RoomWindow::ProductPanel::refresh()
{
	Panel::refresh();

	auto& room = getRoom();

	mIcon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/products/{}.png", room.product)));

	mLevelLabel->setEnabled(isOpened());

	if (mLevelLabel->isEnabled())
		mLevelLabel->setText(getLevelText());
}

int RoomWindow::ProductPanel::getLevel() const
{
	auto& room = getRoom();
	return room.product;
}

bool RoomWindow::ProductPanel::isOpened() const
{
	return getLevel() > 0;
}

bool RoomWindow::ProductPanel::isLastLevel() const
{
	return getLevel() >= Profile::Room::MaxProductLevel;
}

double RoomWindow::ProductPanel::getUpgradeCost() const
{
	return Balance::GetProductCost(getLevel());
}

bool RoomWindow::ProductPanel::isOpenAvailable() const
{
	return true;
}

void RoomWindow::ProductPanel::increaseLevel()
{
	auto room = getRoom();
	room.product += 1;
	PROFILE->setRoom(getRoomIndex(), room);
}

utf8_string RoomWindow::ProductPanel::getOpenButtonText() const
{
	return LOCALIZE("UNLOCK_BUTTON");
}

utf8_string RoomWindow::ProductPanel::getUpgradeButtonText() const
{
	return LOCALIZE("UPGRADE_BUTTON");
}

// small panel

RoomWindow::SmallPanel::SmallPanel(int roomIndex) : Panel(roomIndex)
{
	setTexture(TEXTURE("textures/windows/room_window/minor_panel_backgorund.png"));

	mIcon = std::make_shared<Scene::Sprite>();
	mIcon->setAnchor({ 0.0f, 0.5f });
	mIcon->setPivot({ 0.5f, 0.5f });
	mIcon->setPosition({ 124.0f, 0.0f });
	attach(mIcon);
 
	mTitle = std::make_shared<Helpers::LabelSolid>();
	mTitle->setFontSize(12.0f);
	mTitle->setAnchor({ 0.0f, 0.0f });
	mTitle->setPivot({ 0.0f, 0.5f });
	mTitle->setPosition({ 264.0f, 64.0f });
	mTitle->setColor(Graphics::Color::ToNormalized(12, 22, 44));
	attach(mTitle);

	mLevelLabel = std::make_shared<Helpers::LabelSolid>();
	mLevelLabel->setFontSize(12.0f);
	mLevelLabel->setAnchor({ 0.0f, 0.0f });
	mLevelLabel->setPivot({ 0.0f, 0.5f });
	mLevelLabel->setPosition({ 264.0f, 114.0f });
	mLevelLabel->setColor(Graphics::Color::ToNormalized(23, 0, 164));
	attach(mLevelLabel);

	mHireLabel = std::make_shared<Helpers::LabelSolid>();
	mHireLabel->setFontSize(11.0f);
	mHireLabel->setMultiline(true);
	mHireLabel->setWidth(138.0f);
	mHireLabel->setAnchor({ 0.0f, 0.0f });
	mHireLabel->setPivot({ 0.0f, 0.5f });
	mHireLabel->setPosition({ 264.0f, 142.0f });
	mHireLabel->setColor(Graphics::Color::ToNormalized(12, 22, 44));
	attach(mHireLabel);
}

void RoomWindow::SmallPanel::refresh()
{
	Panel::refresh();

	mIcon->setTexture(getIconTexture());
	mTitle->setText(getTitleText());

	mLevelLabel->setEnabled(isOpened());
	mHireLabel->setEnabled(!isOpened());

	if (mLevelLabel->isEnabled())
		mLevelLabel->setText(getLevelText());

	if (mHireLabel->isEnabled())
		mHireLabel->setText(getHireText());
}

utf8_string RoomWindow::SmallPanel::getOpenButtonText() const
{
	return LOCALIZE("HIRE_BUTTON");
}

utf8_string RoomWindow::SmallPanel::getUpgradeButtonText() const
{
	return LOCALIZE("UPGRADE_BUTTON");
}

// manager panel

RoomWindow::ManagerPanel::ManagerPanel(int roomIndex) : SmallPanel(roomIndex)
{
	//
}

int RoomWindow::ManagerPanel::getLevel() const
{
	auto& room = getRoom();
	return room.manager;
}

bool RoomWindow::ManagerPanel::isOpenAvailable() const
{
	assert(!isOpened());
	auto& room = getRoom();
	return room.worker1 > 0 || room.worker2 > 0 || room.worker3 > 0;
}

bool RoomWindow::ManagerPanel::isOpened() const
{
	return getLevel() > 0;
}

bool RoomWindow::ManagerPanel::isLastLevel() const
{
	return getLevel() >= Profile::Room::MaxManagerLevel;
}

double RoomWindow::ManagerPanel::getUpgradeCost() const
{
	return Balance::GetManagerCost(getLevel());
}

void RoomWindow::ManagerPanel::increaseLevel()
{
	auto room = getRoom();
	room.manager += 1;
	PROFILE->setRoom(getRoomIndex(), room);
}

std::shared_ptr<Renderer::Texture> RoomWindow::ManagerPanel::getIconTexture() const
{
	auto avatar = getLevel();

	if (avatar == 0)
		avatar = 1;

	return TEXTURE(fmt::format("textures/windows/room_window/avatars/managers/{}.png", avatar));
}

utf8_string RoomWindow::ManagerPanel::getTitleText() const
{
	return LOCALIZE("ROOM_WINDOW_MANAGER_TITLE");
}

utf8_string RoomWindow::ManagerPanel::getHireText() const
{
	return LOCALIZE("ROOM_WINDOW_MANAGER_HIRE_DESCRIPTION");
}

// worker panel

RoomWindow::WorkerPanel::WorkerPanel(int roomIndex, int number) : SmallPanel(roomIndex), mNumber(number)
{
	assert(mNumber > 0);
	assert(mNumber < 4);
}

int RoomWindow::WorkerPanel::getLevel() const
{
	auto& room = getRoom();

	if (mNumber == 1)
		return room.worker1;
	else if (mNumber == 2)
		return room.worker2;
	else if (mNumber == 3)
		return room.worker3;

	return 0;
}

bool RoomWindow::WorkerPanel::isOpenAvailable() const
{
	assert(!isOpened());
	auto& room = getRoom();
	return room.product > 0;
}

bool RoomWindow::WorkerPanel::isOpened() const
{
	return getLevel() > 0;
}

bool RoomWindow::WorkerPanel::isLastLevel() const
{
	return getLevel() >= Profile::Room::MaxWorkerLevel;
}

double RoomWindow::WorkerPanel::getUpgradeCost() const
{
	return Balance::GetWorkerCost(getLevel());
}

void RoomWindow::WorkerPanel::increaseLevel() 
{
	auto room = getRoom();

	if (mNumber == 1)
		room.worker1 += 1;
	else if (mNumber == 2)
		room.worker2 += 1;
	else if (mNumber == 3)
		room.worker3 += 1;

	PROFILE->setRoom(getRoomIndex(), room);
}

std::shared_ptr<Renderer::Texture> RoomWindow::WorkerPanel::getIconTexture() const
{
	auto avatar = getLevel();

	if (avatar == 0)
		avatar = 1;

	return TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", avatar));
}

utf8_string RoomWindow::WorkerPanel::getTitleText() const
{
	return LOCALIZE_FMT("ROOM_WINDOW_WORKER_TITLE", mNumber);
}

utf8_string RoomWindow::WorkerPanel::getHireText() const
{
	return LOCALIZE("ROOM_WINDOW_WORKER_HIRE_DESCRIPTION");
}