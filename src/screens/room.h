#pragma once

#include <shared/all.h>
#include "factory.h"
#include "helpers.h"
#include "profile.h"

namespace PhoneInc
{
	class Factory::Room : public Scene::Cullable<Scene::Actionable<Scene::Sprite>>,
		public Common::EventSystem::Listenable<Profile::RoomChangedEvent>
	{
	public:
		class PhonesStack;

	public:
		Room(int index);

	public:
		void refresh();

	public:
		void event(const Profile::RoomChangedEvent& e) override;

	private:
		int mIndex = 0;
		std::shared_ptr<Manager> mManager;
		std::shared_ptr<Worker> mWorker1;
		std::shared_ptr<Worker> mWorker2;
		std::shared_ptr<Worker> mWorker3;
		std::shared_ptr<PhonesStack> mPhonesStack1;
		std::shared_ptr<PhonesStack> mPhonesStack2;
		std::shared_ptr<PhonesStack> mPhonesStack3;
	};

	class Factory::LockedRoom : public Scene::Cullable<Scene::Sprite>,
		public Common::EventSystem::Listenable<Profile::CashChangedEvent>
	{
	public:
		LockedRoom(int index);

	public:
		void event(const Profile::CashChangedEvent& e) override;

	public:
		void refresh();

	private:
		int mIndex = 0;
		std::shared_ptr<Helpers::StandardLongButton> mButton = nullptr;
	};

	class Factory::Room::PhonesStack : public Scene::Clickable<Scene::Node>
	{
	public:
		static const inline int MaxVisiblePhones = 10;

	public:
		PhonesStack(int room_index);

	public:
		void makeProduct();

	public:
		auto getVisiblePhones() const { return mVisiblePhones; }
		void setVisiblePhones(int value);

		bool isFilled() const { return mVisiblePhones >= MaxVisiblePhones; }

	private:
		std::vector<std::shared_ptr<Scene::Sprite>> mPhones;
		int mVisiblePhones = 10;
		int mRoomIndex = 0;
	};
}