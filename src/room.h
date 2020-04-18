#pragma once

#include <shared/all.h>
#include "factory.h"
#include "helpers.h"

namespace PhoneInc
{
	class Factory::Room : public Scene::Actionable<Scene::Sprite>
	{
	public:
		Room(int number);
	};

	class Factory::LockedRoom : public Scene::Sprite
	{
	public:
		LockedRoom(int number);

	public:
		void setUnlockCallback(Helpers::Callback value) { mUnlockCallback = value; }

	private:
		Helpers::Callback mUnlockCallback;
	};
}