#pragma once

#include <shared/all.h>
#include "factory.h"
#include "helpers.h"
#include "profile.h"

namespace PhoneInc
{
	class Factory::Room : public Scene::Cullable<Scene::Actionable<Scene::Sprite>>
	{
	public:
		Room(int index);
	};

	class Factory::LockedRoom : public Scene::Cullable<Scene::Sprite>,
		public Common::EventSystem::Listenable<Profile::CashChangedEvent>
	{
	public:
		LockedRoom(int index);

	public:
		void event(const Profile::CashChangedEvent& e) override;

	private:
		void refresh();

	private:
		int mIndex = 0;
		std::shared_ptr<Helpers::StandardLongButton> mButton = nullptr;
	};
}