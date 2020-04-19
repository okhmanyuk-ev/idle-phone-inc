#pragma once

#include <shared/all.h>

namespace PhoneInc
{
	class Factory : public Scene::Cullable<Scene::Node>
	{
	public:
		class Room;
		class LockedRoom;
		class Worker;
		class Manager;

	public:
		Factory();
	};
}