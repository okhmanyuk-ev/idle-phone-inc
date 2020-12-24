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

std::unique_ptr<Actions::Action> Screen::createEnterAction()
{
	return Actions::Factory::MakeSequence(
		Actions::Factory::WaitOneFrame(),
		Actions::Factory::ChangeAlpha(mFadeRectangle, 0.0f, 0.5f)
	);
};

std::unique_ptr<Actions::Action> Screen::createLeaveAction()
{
	return Actions::Factory::MakeSequence(
		Actions::Factory::WaitOneFrame(),
		Actions::Factory::ChangeAlpha(mFadeRectangle, 1.0f, 0.5f)
	);
};