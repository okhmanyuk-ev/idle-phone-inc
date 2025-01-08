#pragma once

#include <sky/sky.h>
#include "factory.h"
#include "helpers.h"
#include "profile.h"

namespace PhoneInc
{
	class Factory::Room : public Scene::Cullable<Scene::Sprite>,
		public sky::Listenable<Profile::RoomChangedEvent>,
		public sky::Listenable<Profile::CashChangedEvent>
	{
	public:
		class PhonesStack;

	public:
		Room(int index);

	public:
		void refresh();
		void refreshUpgradeButton();

	public:
		void onEvent(const Profile::RoomChangedEvent& e) override;
		void onEvent(const Profile::CashChangedEvent& e) override;

	private:
		int mIndex = 0;
		std::shared_ptr<Manager> mManager;
		std::array<std::shared_ptr<Worker>, Balance::MaxWorkersCount> mWorkers;
		std::array<std::shared_ptr<PhonesStack>, Balance::MaxWorkersCount> mPhonesStacks;
		std::shared_ptr<Helpers::StandardButton> mUpgradeButton;
	};

	class Factory::LockedRoom : public Scene::Cullable<Scene::Sprite>,
		public sky::Listenable<Profile::CashChangedEvent>
	{
	public:
		LockedRoom(int index);

	public:
		void onEvent(const Profile::CashChangedEvent& e) override;

	public:
		void refresh();

	private:
		int mIndex = 0;
		std::shared_ptr<Helpers::StandardLongButton> mButton = nullptr;
		std::shared_ptr<Helpers::DollarEmitter> mDollarEmitter;
	};

	class Factory::Room::PhonesStack : public Scene::Clickable<Scene::Node>
	{
	private:
		using Phone = Scene::Sprite;

	public:
		PhonesStack(int room_index);

	public:
		void makeProduct();

	public:
		auto getVisiblePhones() const { return mVisiblePhones; }
		void setVisiblePhones(int value);

		bool isFilled() const { return mVisiblePhones >= Balance::PhonesStackCount; }

		void runAnimForPhone(int index);

	private:
		std::vector<std::shared_ptr<Phone>> mPhones;
		int mVisiblePhones = 10;
		int mRoomIndex = 0;
	};
}