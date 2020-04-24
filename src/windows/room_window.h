#pragma once

#include "window.h"

namespace PhoneInc
{
	class RoomWindow : public Window,
		public Common::EventSystem::Listenable<Profile::RoomChangedEvent>
	{
	public:
		RoomWindow(int index);

	public:
		void refresh();

	public:
		void event(const Profile::RoomChangedEvent& e) override;

	private:
		struct MajorPanel
		{
			std::shared_ptr<Scene::Sprite> icon;
			std::shared_ptr<Helpers::StandardButton> button;
		};

		struct MinorPanel
		{
			std::shared_ptr<Scene::Sprite> icon;
			std::shared_ptr<Helpers::StandardButton> button;
		};

	private:
		std::shared_ptr<Scene::Node> createMajorPanel(MajorPanel& panel);
		std::shared_ptr<Scene::Node> createMinorPanel(MinorPanel& panel);

	private:
		MajorPanel mProduct;
		MinorPanel mManager;
		MinorPanel mWorker1;
		MinorPanel mWorker2;
		MinorPanel mWorker3;

	private:
		int mIndex = 0;
	};
}
