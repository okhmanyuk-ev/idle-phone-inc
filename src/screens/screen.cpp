#include "screen.h"

using namespace PhoneInc;

Screen::Screen()
{
	setEnabled(false);
	setInteractions(false);
	setStretch(1.0f);
	setAnchor(0.5f);
	setPivot(0.5f);

	mContent = std::make_shared<Scene::Node>();
	mContent->setStretch(1.0f);
	attach(mContent);

	mFadeRectangle = std::make_shared<Scene::Rectangle>();
	mFadeRectangle->setStretch(1.0f);
	mFadeRectangle->setColor(Graphics::Color::Black);
	attach(mFadeRectangle);
}

void Screen::onEnterBegin()
{
	setEnabled(true);
}

void Screen::onEnterEnd()
{
	setInteractions(true);
}

void Screen::onLeaveBegin()
{
	setInteractions(false);
}

void Screen::onLeaveEnd()
{
	setEnabled(false);
}

void Screen::onWindowAppearing()
{
	setInteractions(false);
}

void Screen::onWindowDisappearing()
{
	setInteractions(true);
}

std::unique_ptr<Common::Actions::Action> Screen::createEnterAction()
{
	return Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::WaitOneFrame(),
		Shared::ActionHelpers::ChangeAlpha(mFadeRectangle, 0.0f, 0.5f)
	);
};

std::unique_ptr<Common::Actions::Action> Screen::createLeaveAction()
{
	return Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::WaitOneFrame(),
		Shared::ActionHelpers::ChangeAlpha(mFadeRectangle, 1.0f, 0.5f)
	);
};