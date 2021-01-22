#pragma once

#include <shared/all.h>
#include "microtasks.h"
#include "helpers.h"

namespace PhoneInc
{
	class MicrotasksHolder : public Scene::Node,
		public Common::Event::Listenable<Microtasks::TaskReadyEvent>,
		public Common::Event::Listenable<Microtasks::TaskCompletedEvent>
	{
	public:
		MicrotasksHolder();

	public:
		void start();
		
	private:
		void show();
		void hide(std::function<void()> finishCallback);
		void refresh();
		void refreshWithAnim();

	private:
		void onEvent(const Microtasks::TaskReadyEvent& e) override;
		void onEvent(const Microtasks::TaskCompletedEvent& e) override;

	private:
		std::shared_ptr<Scene::Node> mHolder;
		std::shared_ptr<Scene::Sprite> mBackground;
		std::shared_ptr<Scene::Sprite> mIcon;
		std::shared_ptr<Helpers::Label> mLabel;
		std::shared_ptr<Helpers::StandardLongButton> mRewardButton;
	};
}