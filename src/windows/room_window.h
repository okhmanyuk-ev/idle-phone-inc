#pragma once

#include "window.h"

namespace PhoneInc
{
	class RoomWindow : public Window
	{
	public:
		RoomWindow(int index);

	private:
		std::shared_ptr<Scene::Node> createMajorPanel();
		std::shared_ptr<Scene::Node> createMinorPanel();
	};
}
