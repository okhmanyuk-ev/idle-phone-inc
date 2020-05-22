#include "room_window.h"
#include "balance.h"

using namespace PhoneInc;

RoomWindow::RoomWindow(int index) : mIndex(index)
{
	getBackground()->setSize({ 986.0f, 1658.0f });
	getTitle()->setText(LOCALIZE_FMT("ROOM_WINDOW_TITLE", index + 1));

	mProductPanel = std::make_shared<ProductPanel>(index);
	mProductPanel->setAnchor({ 0.5f, 0.0f });
	mProductPanel->setPivot({ 0.5f, 0.0f });
	mProductPanel->setPosition({ 0.0f, 138.0f });
	getBackground()->attach(mProductPanel);

	mManagerPanel = std::make_shared<ManagerPanel>(index);
	mManagerPanel->setAnchor({ 0.5f, 0.0f });
	mManagerPanel->setPivot({ 0.5f, 0.0f });
	mManagerPanel->setPosition({ 0.0f, 596.0f });
	getBackground()->attach(mManagerPanel);

	mWorkerPanel1 = std::make_shared<WorkerPanel>(index, 1);
	mWorkerPanel1->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel1->setPivot({ 0.5f, 0.0f });
	mWorkerPanel1->setPosition({ 0.0f, 858.0f });
	getBackground()->attach(mWorkerPanel1);

	mWorkerPanel2 = std::make_shared<WorkerPanel>(index, 2);
	mWorkerPanel2->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel2->setPivot({ 0.5f, 0.0f });
	mWorkerPanel2->setPosition({ 0.0f, 1120.0f });
	getBackground()->attach(mWorkerPanel2);

	mWorkerPanel3 = std::make_shared<WorkerPanel>(index, 3);
	mWorkerPanel3->setAnchor({ 0.5f, 0.0f });
	mWorkerPanel3->setPivot({ 0.5f, 0.0f });
	mWorkerPanel3->setPosition({ 0.0f, 1382.0f });
	getBackground()->attach(mWorkerPanel3);

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

bool RoomWindow::Panel::isOpened() const 
{ 
	return getLevel() > 0; 
}

bool RoomWindow::Panel::isLastLevel() const 
{ 
	return getLevel() >= getMaxLevel(); 
}

float RoomWindow::Panel::getProgress() const 
{
	auto current = (((getLevel() - 1) % getLevelsPerStage()) + 1);
	auto total = getLevelsPerStage();
	return (float)current / (float)total;
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

	mTitle = std::make_shared<Helpers::LabelSolid>();
	mTitle->setFontSize(12.0f);
	mTitle->setAnchor({ 0.0f, 0.0f });
	mTitle->setPivot({ 0.0f, 0.5f });
	mTitle->setPosition({ 264.0f, 176.0f });
	mTitle->setColor(Graphics::Color::ToNormalized(12, 22, 44));
	attach(mTitle);

	mDescriptionLabel = std::make_shared<Helpers::LabelSolid>();
	mDescriptionLabel->setFontSize(10.0f);
	mDescriptionLabel->setMultiline(true);
	mDescriptionLabel->setWidth(138.0f);
	mDescriptionLabel->setAnchor({ 0.0f, 0.0f });
	mDescriptionLabel->setPivot({ 0.0f, 0.5f });
	mDescriptionLabel->setPosition({ 264.0f, 254.0f });
	mDescriptionLabel->setColor(Graphics::Color::ToNormalized(12, 22, 44));
	attach(mDescriptionLabel);

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

	auto stage = Balance::GetProductStage(getLevel());

	if (!isOpened())
		mIcon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/products/0.png")));
	else
		mIcon->setTexture(TEXTURE(fmt::format("textures/windows/room_window/avatars/products/{}.png", stage)));

	mLevelLabel->setEnabled(isOpened());
	mDescriptionLabel->setEnabled(!isOpened());

	if (mLevelLabel->isEnabled())
		mLevelLabel->setText(getLevelText());

	if (mDescriptionLabel->isEnabled())
		mDescriptionLabel->setText(LOCALIZE("ROOM_WINDOW_PRODUCT_DESCRIPTION"));

	if (!isOpened())
	{
		mTitle->setText(LOCALIZE("ROOM_WINDOW_PRODUCT_TITLE_OPEN"));
	}
	else
	{
		mTitle->setText(LOCALIZE(fmt::format("ROOM_WINDOW_PRODUCT_TITLE_{}", stage)));
	}

}

int RoomWindow::ProductPanel::getLevel() const
{
	auto& room = getRoom();
	return room.product;
}

int RoomWindow::ProductPanel::getMaxLevel() const
{
	return Balance::MaxProductLevel;
}

int RoomWindow::ProductPanel::getLevelsPerStage() const
{
	return Balance::ProductLevelsPerStage;
}

double RoomWindow::ProductPanel::getUpgradeCost() const
{
	return Balance::GetRoomProductCost(getRoomIndex(), getLevel());
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

	mDescriptionLabel = std::make_shared<Helpers::LabelSolid>();
	mDescriptionLabel->setFontSize(10.0f);
	mDescriptionLabel->setMultiline(true);
	mDescriptionLabel->setWidth(138.0f);
	mDescriptionLabel->setAnchor({ 0.0f, 0.0f });
	mDescriptionLabel->setPivot({ 0.0f, 0.5f });
	mDescriptionLabel->setPosition({ 264.0f, 142.0f });
	mDescriptionLabel->setColor(Graphics::Color::ToNormalized(12, 22, 44));
	attach(mDescriptionLabel);

	mLevelLabel = std::make_shared<Helpers::LabelSolid>();
	mLevelLabel->setFontSize(12.0f);
	mLevelLabel->setAnchor({ 0.0f, 0.0f });
	mLevelLabel->setPivot({ 0.0f, 0.5f });
	mLevelLabel->setPosition({ 264.0f, 114.0f });
	mLevelLabel->setColor(Graphics::Color::ToNormalized(23, 0, 164));
	attach(mLevelLabel);

	mProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mProgressbar->setSize({ 422.0f, 28.0f });
	mProgressbar->setAnchor({ 0.0f, 0.0f });
	mProgressbar->setPivot({ 0.0f, 0.5f });
	mProgressbar->setPosition({ 264.0f, 168.0f });
	attach(mProgressbar);
}

void RoomWindow::SmallPanel::refresh()
{
	Panel::refresh();

	mIcon->setTexture(getIconTexture());
	mTitle->setText(getTitleText());

	mLevelLabel->setEnabled(isOpened());
	mProgressbar->setEnabled(isOpened());
	mDescriptionLabel->setEnabled(!isOpened());

	if (mLevelLabel->isEnabled())
		mLevelLabel->setText(getLevelText());

	if (mProgressbar->isEnabled())
		mProgressbar->setProgress(getProgress());

	if (mDescriptionLabel->isEnabled())
		mDescriptionLabel->setText(getDescriptionText());
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

int RoomWindow::ManagerPanel::getMaxLevel() const
{
	return Balance::MaxManagerLevel;
}

int RoomWindow::ManagerPanel::getLevelsPerStage() const
{
	return Balance::ManagerLevelsPerStage;
}

bool RoomWindow::ManagerPanel::isOpenAvailable() const
{
	assert(!isOpened());
	auto& room = getRoom();
	return room.workers[0] > 0 || room.workers[1] > 0 || room.workers[2] > 0;
}

double RoomWindow::ManagerPanel::getUpgradeCost() const
{
	return Balance::GetRoomManagerCost(getRoomIndex(), getLevel());
}

void RoomWindow::ManagerPanel::increaseLevel()
{
	auto room = getRoom();
	room.manager += 1;
	PROFILE->setRoom(getRoomIndex(), room);
}

std::shared_ptr<Renderer::Texture> RoomWindow::ManagerPanel::getIconTexture() const
{
	auto avatar = Balance::GetManagerStage(getLevel());
	return TEXTURE(fmt::format("textures/windows/room_window/avatars/managers/{}.png", avatar));
}

utf8_string RoomWindow::ManagerPanel::getTitleText() const
{
	if (isOpened())
		return LOCALIZE("ROOM_WINDOW_MANAGER_TITLE");
	else
		return LOCALIZE("ROOM_WINDOW_MANAGER_TITLE_HIRE");
}

utf8_string RoomWindow::ManagerPanel::getDescriptionText() const
{
	if (isOpenAvailable())
		return LOCALIZE("ROOM_WINDOW_MANAGER_DESCRIPTION");
	else
		return LOCALIZE("ROOM_WINDOW_MANAGER_DESCRIPTION_HIRE_LOCKED");
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
	return room.workers[mNumber - 1];
	return 0;
}

int RoomWindow::WorkerPanel::getMaxLevel() const
{
	return Balance::MaxWorkerLevel;
}

int RoomWindow::WorkerPanel::getLevelsPerStage() const
{
	return Balance::WorkerLevelsPerStage;
}

bool RoomWindow::WorkerPanel::isOpenAvailable() const
{
	assert(!isOpened());
	auto& room = getRoom();
	return room.product > 0;
}

double RoomWindow::WorkerPanel::getUpgradeCost() const
{
	return Balance::GetRoomWorkerCost(getRoomIndex(), getLevel(), mNumber);
}

void RoomWindow::WorkerPanel::increaseLevel() 
{
	auto room = getRoom();
	room.workers[mNumber - 1] += 1;
	PROFILE->setRoom(getRoomIndex(), room);
}

std::shared_ptr<Renderer::Texture> RoomWindow::WorkerPanel::getIconTexture() const
{
	auto avatar = Balance::GetWorkerStage(getLevel());
	return TEXTURE(fmt::format("textures/windows/room_window/avatars/workers/{}.png", avatar));
}

utf8_string RoomWindow::WorkerPanel::getTitleText() const
{
	if (isOpened())
		return LOCALIZE_FMT("ROOM_WINDOW_WORKER_TITLE", mNumber);
	else
		return LOCALIZE("ROOM_WINDOW_WORKER_TITLE_HIRE");
}

utf8_string RoomWindow::WorkerPanel::getDescriptionText() const
{
	if (isOpenAvailable())
		return LOCALIZE("ROOM_WINDOW_WORKER_DESCRIPTION");
	else
		return LOCALIZE("ROOM_WINDOW_WORKER_DESCRIPTION_HIRE_LOCKED");
}