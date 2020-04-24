#include "window.h"

using namespace PhoneInc;

Window::Window()
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
	mContent->setScale(Helpers::InvScale);
	attach(mContent);

	setCloseOnMissclick(false);
}

void Window::onOpenEnd()
{
	mContent->setInteractions(true);
}

void Window::onCloseBegin()
{
	mContent->setInteractions(false);
}

std::unique_ptr<Common::Actions::Action> Window::createOpenAction()
{
	return Shared::ActionHelpers::Delayed(0.0f, // do not ruin animation while constructing class
		Shared::ActionHelpers::MakeParallel(
			Shared::ActionHelpers::ChangeAlpha(mFadeRect, 0.5f, 0.5f, Common::Easing::CubicOut),
			Shared::ActionHelpers::ChangeVerticalAnchor(mContent, 0.5f, 0.5f, Common::Easing::CubicOut)
	));
};

std::unique_ptr<Common::Actions::Action> Window::createCloseAction()
{
	return Shared::ActionHelpers::Delayed(0.0f,
		Shared::ActionHelpers::MakeParallel(
			Shared::ActionHelpers::ChangeVerticalAnchor(mContent, -0.5f, 0.5f, Common::Easing::CubicIn),
			Shared::ActionHelpers::ChangeAlpha(mFadeRect, 0.0f, 0.5f, Common::Easing::CubicIn)
	));
};
