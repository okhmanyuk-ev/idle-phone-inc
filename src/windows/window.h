#pragma once

#include <shared/all.h>
#include <helpers.h>

namespace PhoneInc
{
	class Window : public Shared::SceneHelpers::StandardWindow
	{
	public:
		Window();
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
		std::shared_ptr<Helpers::CloseButton> mCloseButton;
	};
}
