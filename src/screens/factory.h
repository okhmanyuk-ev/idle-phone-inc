#pragma once

#include <shared/all.h>
#include "profile.h"

namespace PhoneInc
{
	struct ProductSpawnEvent { int room_index; double produce_count; };

	class Factory : public Scene::ClippableScissor<Scene::Cullable<Scene::Node>>,
		public Common::Event::Listenable<Profile::RoomUnlockedEvent>,
		public Common::Event::Listenable<ProductSpawnEvent>
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
		void onEvent(const Profile::RoomUnlockedEvent& e) override;
		void onEvent(const ProductSpawnEvent& e) override;

	private:
		std::vector<std::shared_ptr<Node>> mRooms;
		std::shared_ptr<Scene::Node> mBoxHolder;
	};

	class Factory::Box : public Scene::Cullable<Scene::Sprite>
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