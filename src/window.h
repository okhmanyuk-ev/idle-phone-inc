#pragma once

#include <Shared/all.h>

namespace PhoneInc
{
	class Window : public Scene::Clickable<Shared::SceneManager::Window>
	{
	public:
		Window()
		{
			setStretch(1.0f);
			setClickCallback([this] {
				if (!mCloseOnMissclick)
					return;

				if (getState() != State::Opened)
					return;

				getSceneManager()->popWindow();
			});

			mFadeRect = std::make_shared<Scene::Rectangle>();
			mFadeRect->setStretch(1.0f);
			mFadeRect->setColor(Graphics::Color::Black);
			mFadeRect->setAlpha(0.0f);
			attach(mFadeRect);
	
			mContent = std::make_shared<Scene::Node>();
			mContent->setStretch(1.0f);
			mContent->setAnchor({ 0.5f, -0.5f });
			mContent->setPivot({ 0.5f, 0.5f });
			mContent->setInteractions(false);
			attach(mContent);
		}

		void onOpenEnd() override
		{
			mContent->setInteractions(true);
		}

		void onCloseBegin() override
		{
			mContent->setInteractions(false);
		}

		std::unique_ptr<Common::Actions::Action> createOpenAction() override
		{
			return Shared::ActionHelpers::MakeParallel(
				Shared::ActionHelpers::ChangeAlpha(mFadeRect, 0.5f, 0.5f, Common::Easing::CubicOut),
				Shared::ActionHelpers::ChangeVerticalAnchor(mContent, 0.5f, 0.5f, Common::Easing::CubicOut)
			);
		};

		std::unique_ptr<Common::Actions::Action> createCloseAction() override
		{
			return Shared::ActionHelpers::MakeParallel(
				Shared::ActionHelpers::ChangeVerticalAnchor(mContent, -0.5f, 0.5f, Common::Easing::CubicIn),
				Shared::ActionHelpers::ChangeAlpha(mFadeRect, 0.0f, 0.5f, Common::Easing::CubicIn)
			);
		};

	public:
		auto getContent() { return mContent; }

		auto getCloseOnMissclick() const { return mCloseOnMissclick; }
		void setCloseOnMissclick(bool value) { mCloseOnMissclick = value; }

	private:
		std::shared_ptr<Scene::Node> mContent;
		std::shared_ptr<Scene::Rectangle> mFadeRect;
		bool mCloseOnMissclick = true;
	};
}
