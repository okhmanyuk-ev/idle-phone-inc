#pragma once

#include <shared/all.h>
#include "profile.h"

namespace PhoneInc
{
	class Factory : public Scene::Clippable<Scene::Cullable<Scene::Node>>,
		public Common::EventSystem::Listenable<Profile::RoomUnlockedEvent>
	{
	public:
		class Room;
		class LockedRoom;
		class Worker;
		class Manager;

	public:
		Factory();

	public:
		void event(const Profile::RoomUnlockedEvent& e) override;

	private:
		std::vector<std::shared_ptr<Node>> mRooms;
	};
}