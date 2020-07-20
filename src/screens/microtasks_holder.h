#pragma once

#include <shared/all.h>
#include "microtasks.h"
#include "helpers.h"

namespace PhoneInc
{
	class MicrotasksHolder : public Scene::Actionable<Scene::Node>,
		public Common::EventSystem::Listenable<Microtasks::TaskCompletedEvent>
	{
	public:
		MicrotasksHolder();

	public:
		void start();
		
	private:
		void show();
		void hide(std::function<void()> finishCallback);
		void refresh();

	private:
		void event(const Microtasks::TaskCompletedEvent& e) override;

	private:
		std::shared_ptr<Scene::Node> mHolder;
		std::shared_ptr<Scene::Sprite> mBackground;
		std::shared_ptr<Scene::Sprite> mIcon;
		std::shared_ptr<Helpers::Label> mLabel;
	};
}