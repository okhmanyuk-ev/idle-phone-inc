#include "helpers.h"

using namespace PhoneInc;
using namespace PhoneInc::Helpers;

Label::Label() : Scene::Label()
{
	setFont(FONT("default"));
	setOutlineThickness(0.5f);
	setScale(Scale);
}

LabelSolid::LabelSolid()
{
	setOutlineThickness(0.0f);
}

Button::Button()
{
	setActive(true);

	mLabel = std::make_shared<Label>();
	mLabel->setPivot(0.5f);
	mLabel->setAnchor(0.5f);
	mLabel->setFontSize(12.0f);
	mLabel->setY(-12.0f);
	attach(mLabel);
}

void Button::setActive(bool value)
{
	mActive = value;

	if (mActive)
		setTexture(TEXTURE("textures/button_active.png"));
	else
		setTexture(TEXTURE("textures/button_inactive.png"));
}
