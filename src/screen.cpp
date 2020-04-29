#include "screen.h"

using namespace PhoneInc;

Screen::Screen()
{
	setEnabled(false);
	setInteractions(false);
	setStretch(1.0f);
	setAnchor(0.5f);
	setPivot(0.5f);
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
	//	setAlpha(0.0f);
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
	return nullptr;
};

std::unique_ptr<Common::Actions::Action> Screen::createLeaveAction()
{
	return nullptr;
};