#pragma once

#include <shared/all.h>

namespace PhoneInc
{
	class Screen : public Shared::SceneManager::Screen, public std::enable_shared_from_this<Screen>
	{
	public:
		Screen();

	public:
		auto getContent() const { return mContent; }

	protected:
		void onEnterBegin() override;
		void onEnterEnd() override;
		void onLeaveBegin() override;
		void onLeaveEnd() override;
		void onWindowAppearing() override;
		void onWindowDisappearing() override;

	protected:
		std::unique_ptr<Actions::Action> createEnterAction() override;
		std::unique_ptr<Actions::Action> createLeaveAction() override;

	private:
		std::shared_ptr<Scene::Node> mContent;
		std::shared_ptr<Scene::Rectangle> mFadeRectangle;
	};
}