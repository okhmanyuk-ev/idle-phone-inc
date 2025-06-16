#include "room.h"
#include "worker.h"
#include "manager.h"
#include "balance.h"
#include "windows/room_window.h"
#include "tutor.h"

using namespace PhoneInc;

Factory::Room::Room(int index) : mIndex(index)
{
	setBatchGroup("room");

	const int BgCount = 7;

	auto bg_id = (index % BgCount) + 1;
	auto table_id = ((index / 2) % 3) + 1;

	setTexture(sky::GetTexture(fmt::format("textures/factory/room/background/{}.png", bg_id)));

	auto table = std::make_shared<Scene::Sprite>();
	table->setTexture(sky::GetTexture(fmt::format("textures/factory/room/table/{}.png", table_id)));
	table->setAnchor({ 0.5f, 1.0f });
	table->setPivot({ 0.5f, 1.0f });
	table->setY(-56.0f);
	table->setBatchGroup("room_table");
	attach(table);

	auto room_profile = PROFILE->getRooms().at(index);

	mManager = std::make_shared<Manager>();
	mManager->setPosition({ -18.0f, 116.0f });
	attach(mManager);

	for (int i = 0; i < Balance::MaxWorkersCount; i++)
	{
		mWorkers[i] = std::make_shared<Worker>();
		mWorkers[i]->setAnchor({ 0.5f, 0.0f });
		mWorkers[i]->setPivot({ 0.5f, 1.0f });
		table->attach(mWorkers[i]);
	}

	mWorkers[0]->setPosition({ -152.0f, 24.0f });
	mWorkers[1]->setPosition({ 0.0f, 24.0f });
	mWorkers[2]->setPosition({ 152.0f, 24.0f });

	auto lvl_label = std::make_shared<Helpers::LabelSolidBold>();
	lvl_label->setBatchGroup("room_lvl_label");
	lvl_label->setPosition({ 51.0f, 59.0f });
	lvl_label->setPivot(0.5f);
	lvl_label->setText(std::to_wstring(index + 1));
	lvl_label->setColor(sky::GetColor(sky::Color::Black));
	lvl_label->setFontSize(33.0f);
	attach(lvl_label);

	mUpgradeButton = std::make_shared<Helpers::StandardButton>();
	mUpgradeButton->setBatchGroup("room_button");
	mUpgradeButton->setPosition({ 681.0f, 28.0f });
	mUpgradeButton->getLabel()->setText(sky::Localize("UPGRADE_BUTTON"));
	mUpgradeButton->setClickCallback([index] {
		SCENE_MANAGER->pushWindow(std::make_shared<RoomWindow>(index));
		TUTOR->complete();
	});
	mUpgradeButton->setTouchTransparent(true);
	attach(mUpgradeButton);

	if (index == 0)
	{
		auto beginCallback = [] {
			sky::Emit(Helpers::BlockGlobalScrollEvent({ true }));
		};
		auto endCallback = [] {
			sky::Emit(Helpers::BlockGlobalScrollEvent({ false }));
		};
		TUTOR->play("press_upgrade_room_button", mUpgradeButton, nullptr, beginCallback, endCallback);
	}

	for (int i = 0; i < Balance::MaxWorkersCount; i++)
	{
		mPhonesStacks[i] = std::make_shared<PhonesStack>(index);
		mPhonesStacks[i]->setPivot(0.5f);
		mPhonesStacks[i]->setVisiblePhones(0);
		table->attach(mPhonesStacks[i]);
	}

	mPhonesStacks[0]->setPosition({ 46.0f, -12.0f });
	mPhonesStacks[1]->setPosition({ 196.0f, -12.0f });
	mPhonesStacks[2]->setPosition({ 346.0f, -12.0f });

	refresh();

	runAction(Actions::Collection::RepeatInfinite([this]() -> std::unique_ptr<Actions::Action> {
		const auto& rooms = PROFILE->getRooms();

		if (rooms.count(mIndex) == 0)
			return nullptr;

		auto room = rooms.at(mIndex);

		if (room.manager == 0)
			return nullptr;

		bool hasFilled = false;
		for (auto phones_stack : mPhonesStacks)
		{
			if (!phones_stack->isFilled())
				continue;

			hasFilled = true;
			break;
		}

		if (!hasFilled)
			return nullptr;

		auto duration = Balance::GetManagerDuration(mIndex);

		return Actions::Collection::MakeSequence(
			Actions::Collection::Execute([this] {
				mManager->setStateType(ManagerAnimation::Working);
				mManager->getProgressbar()->setEnabled(true);
				mManager->getProgressbar()->setProgress(0.0f);
			}),
			Actions::Collection::ChangeScale(mManager->getProgressbar(), { 1.0f, 1.0f }, 0.125f, Easing::BackOut),
			Actions::Collection::Interpolate(0.0f, 1.0f, duration, Easing::Linear, [this](float value) {
				mManager->getProgressbar()->setProgress(value);
			}), 
			Actions::Collection::ChangeScale(mManager->getProgressbar(), { 0.0f, 0.0f }, 0.125f, Easing::BackIn),
			Actions::Collection::Execute([this] {
				mManager->getProgressbar()->setEnabled(false);
				mManager->setStateType(ManagerAnimation::Idle);
				for (auto phones_stack : mPhonesStacks)
				{
					if (!phones_stack->isFilled())
						continue;

					phones_stack->makeProduct();
				}
			})
		);
	}));

	for (int i = 0; i < Balance::MaxWorkersCount; i++)
	{
		runAction(Actions::Collection::RepeatInfinite([this, i]() -> std::unique_ptr<Actions::Action> {
			const auto& rooms = PROFILE->getRooms();

			if (rooms.count(mIndex) == 0)
				return nullptr;

			auto room = rooms.at(mIndex);

			if (room.workers[i] <= 0)
				return nullptr;

			auto duration = Balance::GetWorkerDuration(mIndex, i);
			auto phone = mPhonesStacks.at(i);

			return Actions::Collection::Delayed(duration, Actions::Collection::Execute([this, phone] {
				if (phone->isFilled())
					return;

				auto count = phone->getVisiblePhones() + 1;

				phone->setVisiblePhones(count);
				phone->runAnimForPhone(count - 1);
			}));
		}));
	}

	runAction(Actions::Collection::ExecuteInfinite([this] {
		for (int i = 0; i < Balance::MaxWorkersCount; i++)
		{
			mWorkers[i]->setStateType(mPhonesStacks[i]->isFilled() ? Worker::Animation::Idle : Worker::Animation::Working);
		}
	}));

	if (mIndex == 0)
	{
		auto canStartCallback = [this] {
			return mPhonesStacks.at(0)->isFilled() && !SCENE_MANAGER->hasWindows();
		};
		auto beginCallback = [] {
			sky::Emit(Helpers::MoveGlobalScrollEvent({ { 0.0f, 0.0f } }));
			sky::Emit(Helpers::BlockGlobalScrollEvent({ true }));
		};
		auto endCallback = [] {
			sky::Emit(Helpers::BlockGlobalScrollEvent({ false }));
		};
		TUTOR->play("press_phones_stack", mPhonesStacks.at(0), canStartCallback, beginCallback, endCallback);
	}
}

void Factory::Room::onEvent(const Profile::RoomChangedEvent& e)
{
	if (e.index != mIndex)
		return;

	refresh();
}

void Factory::Room::onEvent(const Profile::CashChangedEvent& e)
{
	refreshUpgradeButton();
}

void Factory::Room::refresh()
{
	auto room = PROFILE->getRooms().at(mIndex);

	mManager->setLevel(room.manager);
	for (int i = 0; i < Balance::MaxWorkersCount; i++)
	{
		mWorkers[i]->setLevel(room.workers[i]);
	}

	refreshUpgradeButton();
}

void Factory::Room::refreshUpgradeButton()
{
	mUpgradeButton->setActive(RoomWindow::CanUpgradeSomething(mIndex));
}

Factory::LockedRoom::LockedRoom(int index) : mIndex(index)
{
	setBatchGroup("room");
	setTexture(sky::GetTexture("textures/factory/room/background/locked.png"));

	auto lvl_label = std::make_shared<Helpers::LabelSolidBold>();
	lvl_label->setBatchGroup("room_lvl_label");
	lvl_label->setPosition({ 51.0f, 59.0f });
	lvl_label->setPivot(0.5f);
	lvl_label->setText(std::to_wstring(index + 1));
	lvl_label->setColor(sky::GetColor(sky::Color::Black));
	lvl_label->setFontSize(33.0f);
	attach(lvl_label);

	mButton = std::make_shared<Helpers::StandardLongButton>();
	mButton->setBatchGroup("room_button");
	mButton->getLabel()->setText(L"$ " + Helpers::NumberToString(Balance::GetRoomCost(index)));
	mButton->setAnchor(0.5f);
	mButton->setPivot(0.5f);
	mButton->setTouchTransparent(true);
	mButton->setActiveCallback([this] {
		sky::PlaySound(sky::GetSound("sounds/success.wav"));
		mDollarEmitter->emitPack();
		TUTOR->complete();
		PROFILE->spendCash(Balance::GetRoomCost(mIndex));
		PROFILE->unlockRoom(mIndex);
	});
	attach(mButton);

	mDollarEmitter = std::make_shared<Helpers::DollarEmitter>();
	mButton->attach(mDollarEmitter);

	refresh();

	if (index == 0)
	{
		auto beginCallback = [] {
			sky::Emit(Helpers::BlockGlobalScrollEvent({ true }));
		};
		auto endCallback = [] {
			sky::Emit(Helpers::BlockGlobalScrollEvent({ false }));
		};
		TUTOR->play("unlock_room", mButton, nullptr, beginCallback, endCallback);
	}
}

void Factory::LockedRoom::onEvent(const Profile::CashChangedEvent& e)
{
	refresh();
}

void Factory::LockedRoom::refresh()
{
	mButton->setActive(PROFILE->isEnoughCash(Balance::GetRoomCost(mIndex)));
}

Factory::Room::PhonesStack::PhonesStack(int room_index) : mRoomIndex(room_index)
{
	setSize({ 124.0f, 96.0f });
	setTouchTransparent(true);

	for (int i = -5; i < 5; i++)
	{
		auto phone = std::make_shared<Phone>();
		phone->setBatchGroup("room_phonestack");
		phone->setTexture(sky::GetTexture("textures/factory/room/phone.png"));
		phone->setPivot(0.5f);
		phone->setAnchor(0.5f);
		phone->setY(-i * 6.0f);
		attach(phone);
		mPhones.push_back(phone);
	}

	setClickCallback([this] {
		if (!isFilled())
			return;

		makeProduct();
		TUTOR->complete();
		sky::PlaySound("sounds/click.wav");
	});
}

void Factory::Room::PhonesStack::makeProduct()
{
	const auto& rooms = PROFILE->getRooms();

	if (rooms.count(mRoomIndex) == 0)
		return;

	const auto& room = rooms.at(mRoomIndex);

	auto produce_count = Balance::GetRoomProduceCount(mRoomIndex, room.product);

	setVisiblePhones(0);
	sky::Emit(ProductSpawnEvent({ mRoomIndex, produce_count }));
}

void Factory::Room::PhonesStack::setVisiblePhones(int value)
{
	assert(value >= 0);
	assert(value <= Balance::PhonesStackCount);

	mVisiblePhones = value;

	for (int i = 0; i < mPhones.size(); i++)
	{
		mPhones.at(i)->setEnabled(i < value);
	}

	for (auto phone : mPhones)
	{
		if (isFilled())
			phone->setColor(sky::GetColor(sky::Color::Lime) * 3.0f);
		else
			phone->setColor(sky::GetColor(sky::Color::White));
	}
}

void Factory::Room::PhonesStack::runAnimForPhone(int index)
{
	auto phone = mPhones.at(index);

	phone->setScale(0.0f);
	phone->setVerticalOrigin(64.0f);

	phone->runAction(Actions::Collection::MakeParallel(
		Actions::Collection::ChangeScale(phone, { 1.0f, 1.0f }, 0.125f, Easing::BackOut),
		Actions::Collection::ChangeVerticalOrigin(phone, 0.0f, 0.25f, Easing::CubicOut)
	));
}