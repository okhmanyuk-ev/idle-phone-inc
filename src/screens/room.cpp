#include "room.h"
#include "worker.h"
#include "manager.h"
#include "balance.h"
#include "windows/room_window.h"

using namespace PhoneInc;

Factory::Room::Room(int index) : mIndex(index)
{
	auto bg_id = (index % 3) + 1;
	auto table_id = ((index / 2) % 3) + 1;

	setTexture(TEXTURE(fmt::format("textures/factory/room/background/{}.png", bg_id)));

	auto table = std::make_shared<Scene::Sprite>();
	table->setTexture(TEXTURE(fmt::format("textures/factory/room/table/{}.png", table_id)));
	table->setAnchor({ 0.5f, 1.0f });
	table->setPivot({ 0.5f, 1.0f });
	table->setY(-56.0f);
	attach(table);

	auto room_profile = PROFILE->getRooms().at(index);

	mManager = std::make_shared<Manager>();
	mManager->setPosition({ -18.0f, 116.0f });
	attach(mManager);

	mWorker1 = std::make_shared<Worker>();
	mWorker1->setAnchor({ 0.5f, 0.0f });
	mWorker1->setPivot({ 0.5f, 1.0f });
	mWorker1->setPosition({ -152.0f, 24.0f });
	table->attach(mWorker1);
	
	mWorker2 = std::make_shared<Worker>();
	mWorker2->setAnchor({ 0.5f, 0.0f });
	mWorker2->setPivot({ 0.5f, 1.0f });
	mWorker2->setPosition({ 0.0f, 24.0f });
	table->attach(mWorker2);
	
	mWorker3 = std::make_shared<Worker>();
	mWorker3->setAnchor({ 0.5f, 0.0f });
	mWorker3->setPivot({ 0.5f, 1.0f });
	mWorker3->setPosition({ 152.0f, 24.0f });
	table->attach(mWorker3);

	auto lvl_label = std::make_shared<Helpers::LabelSolidBold>();
	lvl_label->setPosition({ 51.0f, 59.0f });
	lvl_label->setPivot(0.5f);
	lvl_label->setText(std::to_string(index + 1));
	lvl_label->setColor(Graphics::Color::Black);
	lvl_label->setFontSize(11.0f);
	attach(lvl_label);

	auto upgrade_btn = std::make_shared<Helpers::StandardButton>();
	upgrade_btn->setPosition({ 681.0f, 28.0f });
	upgrade_btn->getLabel()->setText(LOCALIZE("UPGRADE_BUTTON"));
	upgrade_btn->setClickCallback([index] {
		auto window = std::make_shared<RoomWindow>(index);
		EVENT->emit(Helpers::PushWindowEvent({ window }));
	});
	attach(upgrade_btn);

	mPhonesStack1 = std::make_shared<PhonesStack>(index);
	mPhonesStack1->setPivot(0.5f);
	mPhonesStack1->setPosition({ 46.0f, -12.0f });
	mPhonesStack1->setVisiblePhones(0);
	table->attach(mPhonesStack1);

	mPhonesStack2 = std::make_shared<PhonesStack>(index);
	mPhonesStack2->setPivot(0.5f);
	mPhonesStack2->setPosition({ 196.0f, -12.0f });
	mPhonesStack2->setVisiblePhones(0);
	table->attach(mPhonesStack2);

	mPhonesStack3 = std::make_shared<PhonesStack>(index);
	mPhonesStack3->setPivot(0.5f);
	mPhonesStack3->setPosition({ 346.0f, -12.0f });
	mPhonesStack3->setVisiblePhones(0);
	table->attach(mPhonesStack3);

	refresh();

	runAction(Shared::ActionHelpers::RepeatInfinite([this] {
		return Shared::ActionHelpers::Delayed(5.0f, Shared::ActionHelpers::Execute([this] {
			auto room = PROFILE->getRooms().at(mIndex);

			if (room.manager == 0)
				return;

			if (mPhonesStack1->isFilled())
				mPhonesStack1->makeProduct();

			if (mPhonesStack2->isFilled())
				mPhonesStack2->makeProduct();

			if (mPhonesStack3->isFilled())
				mPhonesStack3->makeProduct();
		}));
	}));

	runAction(Shared::ActionHelpers::RepeatInfinite([this] {
		return Shared::ActionHelpers::Delayed(1.0f, Shared::ActionHelpers::Execute([this] {
			auto room = PROFILE->getRooms().at(mIndex);

			if (room.worker1 > 0 && mPhonesStack1->getVisiblePhones() < PhonesStack::MaxVisiblePhones)
				mPhonesStack1->setVisiblePhones(mPhonesStack1->getVisiblePhones() + 1);

			if (room.worker2 > 0 && mPhonesStack2->getVisiblePhones() < PhonesStack::MaxVisiblePhones)
				mPhonesStack2->setVisiblePhones(mPhonesStack2->getVisiblePhones() + 1);

			if (room.worker3 > 0 && mPhonesStack3->getVisiblePhones() < PhonesStack::MaxVisiblePhones)
				mPhonesStack3->setVisiblePhones(mPhonesStack3->getVisiblePhones() + 1);
		}));
	}));

	runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
		mWorker1->setStateType(mPhonesStack1->isFilled() ? Worker::Animation::Idle : Worker::Animation::Working);
		mWorker2->setStateType(mPhonesStack2->isFilled() ? Worker::Animation::Idle : Worker::Animation::Working);
		mWorker3->setStateType(mPhonesStack3->isFilled() ? Worker::Animation::Idle : Worker::Animation::Working);
	}));
}

void Factory::Room::event(const Profile::RoomChangedEvent& e)
{
	if (e.index != mIndex)
		return;

	refresh();
}

void Factory::Room::refresh()
{
	auto room = PROFILE->getRooms().at(mIndex);

	mManager->setLevel(room.manager);
	mWorker1->setLevel(room.worker1);
	mWorker2->setLevel(room.worker2);
	mWorker3->setLevel(room.worker3);
}

Factory::LockedRoom::LockedRoom(int index) : mIndex(index)
{
	setTexture(TEXTURE("textures/factory/room/background/locked.png"));

	mButton = std::make_shared<Helpers::StandardLongButton>();
	mButton->getLabel()->setText("$ " + Helpers::NumberToString(Balance::GetRoomCost(index)));
	mButton->setAnchor(0.5f);
	mButton->setPivot(0.5f);
	mButton->setActiveCallback([this] {
		PROFILE->spendCash(Balance::GetRoomCost(mIndex));
		PROFILE->unlockRoom(mIndex);
	});
	attach(mButton);
	refresh();
}

void Factory::LockedRoom::event(const Profile::CashChangedEvent& e)
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

	for (int i = -5; i < 5; i++)
	{
		auto phone = std::make_shared<Scene::Sprite>();
		phone->setTexture(TEXTURE("textures/factory/room/phone.png"));
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
	});
}

void Factory::Room::PhonesStack::makeProduct()
{
	setVisiblePhones(0);
	EVENT->emit(ProductSpawnEvent({ mRoomIndex }));
}

void Factory::Room::PhonesStack::setVisiblePhones(int value)
{
	assert(value >= 0);
	assert(value <= MaxVisiblePhones);

	mVisiblePhones = value;
	
	for (int i = 0; i < mPhones.size(); i++)
	{
		mPhones.at(i)->setEnabled(i < value);
	}

	for (auto phone : mPhones)
	{
		if (isFilled())
			phone->setColor(Graphics::Color::Lime);
		else
			phone->setColor(Graphics::Color::White);
	}
}