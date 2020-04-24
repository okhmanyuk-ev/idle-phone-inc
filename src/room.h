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
}