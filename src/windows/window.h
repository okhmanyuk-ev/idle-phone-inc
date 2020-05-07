#pragma once

#include <Shared/all.h>
#include <helpers.h>

namespace PhoneInc
{
	class Window : public Scene::Clickable<Shared::SceneManager::Window>
	{
	public:
		Window();
		
	public:
		void onOpenEnd() override;
		void onCloseBegin() override;

	public:
		std::unique_ptr<Common::Actions::Action> createOpenAction() override;
		std::unique_ptr<Common::Actions::Action> createCloseAction() override;

	public:
		auto getContent() { return mContent; }

		auto getCloseOnMissclick() const { return mCloseOnMissclick; }
		void setCloseOnMissclick(bool value) { mCloseOnMissclick = value; }

	private:
		std::shared_ptr<Scene::Node> mContent;
		std::shared_ptr<Scene::Rectangle> mFadeRect;
		bool mCloseOnMissclick = true;
	};

	class StandardWindow : public Window
	{
	public:
		StandardWindow();

	public:
		virtual void refresh();

	public:
		auto getBackground() { return mBackground; }
		auto getTitle() { return mTitle; }

	private:
		std::shared_ptr<Scene::SlicedSprite> mBackground;
		std::shared_ptr<Helpers::LabelSolid> mTitle;
	};
}
