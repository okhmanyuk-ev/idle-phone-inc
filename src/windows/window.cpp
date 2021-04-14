#include "window.h"
#include "helpers.h"

using namespace PhoneInc;

Window::Window()
{
	setStretch(1.0f);
	setClickCallback([this] {
		if (!mCloseOnMissclick)
			return;

		if (getState() != State::Opened)
			return;

		SCENE_MANAGER->popWindow();
	});

	getBackshadeColor()->setColor({ Graphics::Color::Black, 0.0f });

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

std::unique_ptr<Actions::Action> Window::createOpenAction()
{
	return Actions::Collection::MakeSequence(
		Actions::Collection::WaitOneFrame(),
		Actions::Collection::MakeParallel(
			Actions::Collection::ChangeAlpha(getBackshadeColor(), 0.5f, 0.5f, Easing::CubicOut),
			Actions::Collection::ChangeVerticalAnchor(mContent, 0.5f, 0.5f, Easing::CubicOut)
		)
	);
};

std::unique_ptr<Actions::Action> Window::createCloseAction()
{
	return Actions::Collection::MakeSequence(
		Actions::Collection::WaitOneFrame(),
		Actions::Collection::MakeParallel(
			Actions::Collection::ChangeVerticalAnchor(mContent, -0.5f, 0.5f, Easing::CubicIn),
			Actions::Collection::ChangeAlpha(getBackshadeColor(), 0.0f, 0.5f, Easing::CubicIn)
		)
	);
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
	mTitle->setFontSize(45.0f);
	mTitle->setPosition({ 0.0f, 64.0f });
	mTitle->setText("title");
	getBackground()->attach(mTitle);

	mCloseButton = std::make_shared<Helpers::CloseButton>();
	mCloseButton->setPivot(0.5f);
	mCloseButton->setAnchor({ 1.0f, 0.0f });
	mCloseButton->setPosition({ -72.0f, 72.0f });
	getBackground()->attach(mCloseButton);
}