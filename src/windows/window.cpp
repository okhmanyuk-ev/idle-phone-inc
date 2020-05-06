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
	attach(mContent);
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

StandardWindow::StandardWindow()
{
	mBackground = std::make_shared<Scene::SlicedSprite>();
	mBackground->setTexture(TEXTURE("textures/windows/background_slice.png"));
	mBackground->setCenterRegion({ { 14.0f, 131.0f }, { 30.0f, 17.0f } });
	mBackground->setAnchor(0.5f);
	mBackground->setPivot(0.5f);
	mBackground->setTouchable(true);
	getContent()->attach(mBackground);

	mTitle = std::make_shared<Helpers::LabelSolid>();
	mTitle->setAnchor({ 0.5f, 0.0f });
	mTitle->setPivot(0.5f);
	mTitle->setFontSize(15.0f);
	mTitle->setPosition({ 0.0f, 64.0f });
	mTitle->setText(LOCALIZE("STANDARD_WINDOW_TITLE"));
	getBackground()->attach(mTitle);

	auto close = std::make_shared<Helpers::CloseButtonWidget>();
	close->setPivot(0.5f);
	close->setAnchor({ 1.0f, 0.0f });
	close->setPosition({ -72.0f, 72.0f });
	getBackground()->attach(close);
}