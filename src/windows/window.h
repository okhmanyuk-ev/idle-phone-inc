#pragma once

#include <Shared/all.h>
#include <helpers.h>

namespace PhoneInc
{
	class Window : public Scene::Clickable<Shared::SceneHelpers::Backshaded<Shared::SceneManager::Window>>
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
		bool mCloseOnMissclick = true;
	};

	class StandardWindow : public Window
	{
	public:
		StandardWindow();

	public:
		auto getBackground() { return mBackground; }
		auto getTitle() { return mTitle; }
		auto getCloseButton() { return mCloseButton; }

	private:
		std::shared_ptr<Scene::SlicedSprite> mBackground;
		std::shared_ptr<Helpers::LabelSolid> mTitle;
		std::shared_ptr<Helpers::CloseButtonWidget> mCloseButton;
	};
}
