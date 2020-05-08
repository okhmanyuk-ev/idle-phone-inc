#pragma once

#include <shared/all.h>
#include "profile.h"

namespace PhoneInc
{
	struct ProductSpawnEvent { int room_index; };

	class Factory : public Scene::Actionable<Scene::Clippable<Scene::Cullable<Scene::Node>>>,
		public Common::EventSystem::Listenable<Profile::RoomUnlockedEvent>,
		public Common::EventSystem::Listenable<ProductSpawnEvent>
	{
	public:
		static inline const float ConveyorSpeed = 1.0f;

	public:
		class Room;
		class LockedRoom;
		class Worker;
		class Manager;
		class Box;

	public:
		Factory();

	public:
		void event(const Profile::RoomUnlockedEvent& e) override;
		void event(const ProductSpawnEvent& e) override;

	private:
		std::vector<std::shared_ptr<Node>> mRooms;
		std::shared_ptr<Scene::Node> mBoxHolder;
	};

	class Factory::Box : public Scene::Actionable<Scene::Cullable<Scene::Sprite>>
	{
	public:
		auto getCount() const { return mCount; }
		void setCount(double value) { mCount = value; }

		auto isSpawnAnimationCompleted() const { return mSpawnAnimationCompleted; }
		void setSpawnAnimationCompleted(bool value) { mSpawnAnimationCompleted = value; }

	private:
		double mCount = 0.0;
		bool mSpawnAnimationCompleted = false;
	};
}