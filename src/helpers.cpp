#include "helpers.h"
#include <iomanip>

using namespace PhoneInc;
using namespace PhoneInc::Helpers;

// static

std::string Helpers::NumberToString(double value)
{
	auto frexp10 = [](double arg, int& exp) -> double {
		exp = (arg == 0) ? 0 : (int)std::floor(std::log10(std::fabs(arg)));
		return arg * std::pow(10, -exp);
	};

	auto getExpName = [](int exp) ->std::string {
		const std::map<int, std::string> Names = {
			{ 3, "K" },
			{ 6, "M" },
			{ 9, "B" },
			{ 12, "T" },
			{ 15, "q" },
			{ 18, "Q" },
			{ 21, "s" },
			{ 24, "S" },
			{ 27, "O" },
			{ 30, "N" },
			{ 33, "d" },
			{ 36, "U" },
			{ 39, "D" }
		};

		if (exp > Names.rbegin()->first)
		{
			assert(exp % 3 == 0);
			return "e" + std::to_string(exp);
		}
		else
		{
			assert(Names.count(exp) > 0);
			return Names.at(exp);
		}
	};

	int exp = 0;
	auto normalized_value = frexp10(value, exp);

	std::ostringstream stream;

	stream << std::fixed;

	if (exp < 3)
	{
		stream << std::setprecision(0);
		stream << value;
	}
	else
	{
		auto k = exp % 3;

		stream << std::setprecision(2 - k);
		normalized_value *= glm::pow(10, k);
		exp -= k;

		stream << normalized_value << getExpName(exp);
	}

	return stream.str();
}

// classes

Label::Label() : Scene::Label()
{
	setFont(FONT("default"));
	setOutlineThickness(0.5f);
}

LabelSolid::LabelSolid()
{
	setOutlineThickness(0.0f);
}

LabelSolidBold::LabelSolidBold()
{
	setFont(FONT("default_bold"));
}

void Button::update()
{
	Scene::Clickable<Scene::Sprite>::update();

	if (!mAutoclick)
		return;

	if (!isChoosed())
		return;

	mNextAutoclick -= Clock::ToSeconds(FRAME->getTimeDelta());

	if (mNextAutoclick > 0.0f)
		return;

	internalClick();

	mAutoclickCount += 1;
	mNextAutoclick = MaxAutoclickTime / ((float)mAutoclickCount / 1.5f);
}

void Button::onClick()
{
	if (!mAutoclick)
	{
		internalClick();
	}
	else
	{
		// nothing here
		// disable default click behaviour
	}
}

void Button::onChooseBegin()
{
	Scene::Clickable<Scene::Sprite>::onChooseBegin();
	Shared::SceneHelpers::RecursiveColorSet(shared_from_this(), glm::vec4(1.25f));

	if (mAutoclick)
	{
		internalClick();
		mNextAutoclick = MaxAutoclickTime;
		mAutoclickCount = 1;
	}
}

void Button::onChooseEnd()
{
	Scene::Clickable<Scene::Sprite>::onChooseEnd();
	Shared::SceneHelpers::RecursiveColorSet(shared_from_this(), glm::vec4(1.0f));
}

void Button::internalClick()
{
	Scene::Clickable<Scene::Sprite>::onClick();

	auto executeCallback = [](auto callback) { if (callback) callback(); };
	if (isActive())
	{
		executeCallback(mActiveCallback);
		AUDIO->play(mActiveSound);
	}
	else
	{
		executeCallback(mInactiveCallback);
		AUDIO->play(mInactiveSound);
	}
}

void Button::setActive(bool value)
{
	mActive = value;
	ensureTexture();
}

void Button::setActiveTexture(Graphics::TexCell value)
{
	mActiveTexture = value;
	ensureTexture();
}

void Button::setInactiveTexture(Graphics::TexCell value)
{
	mInactiveTexture = value;
	ensureTexture();
}

void Button::ensureTexture()
{
	if (mActive)
		setTexture(mActiveTexture);
	else
		setTexture(mInactiveTexture);
}

StandardButton::StandardButton() : Button()
{
	setActiveTexture(TEXTURE("textures/buttons/button_active.png"));
	setInactiveTexture(TEXTURE("textures/buttons/button_inactive.png"));

	mLabel = std::make_shared<Label>();
	mLabel->setPivot(0.5f);
	mLabel->setAnchor(0.5f);
	mLabel->setFontSize(36.0f);
	mLabel->setY(-12.0f);
	attach(mLabel);
}

StandardLongButton::StandardLongButton() : Button()
{
	setActiveTexture(TEXTURE("textures/buttons/button_long_active.png"));
	setInactiveTexture(TEXTURE("textures/buttons/button_long_inactive.png"));

	mLabel = std::make_shared<Label>();
	mLabel->setPivot(0.5f);
	mLabel->setAnchor(0.5f);
	mLabel->setFontSize(36.0f);
	mLabel->setY(-12.0f);
	attach(mLabel);
}

CloseButtonWidget::CloseButtonWidget() : Button()
{
	setTexture(TEXTURE("textures/buttons/close.png"));
	setClickCallback([] {
		SCENE_MANAGER->popWindow();
	});
}

Progressbar::Progressbar()
{
	mHolder = std::make_shared<Scene::Node>();
	mHolder->setAnchor(0.5f);
	mHolder->setPivot(0.5f);
	mHolder->setStretch(1.0f);
	attach(mHolder);

	mClip = std::make_shared<Scene::ClippableScissor<Scene::Node>>();
	mClip->setStretch(1.0f);
	mClip->setAnchor({ 0.0f, 0.5f });
	mClip->setPivot({ 0.0f, 0.5f });
	mHolder->attach(mClip);

	mContent = std::make_shared<Scene::SlicedSprite>();
	mContent->setStretch({ -1.0f, 1.0f });
	mContent->setAnchor({ 0.0f, 0.5f });
	mContent->setPivot({ 0.0f, 0.5f });
	mClip->attach(mContent);

	runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
		mContent->setWidth(mHolder->getWidth());
		mContent->setSampler(getSampler());
	}));
}

StreetProgressbar::StreetProgressbar() : Progressbar()
{
	setTexture(TEXTURE("textures/ui/progressbar_background.png"));
	setCenterRegion({ { 8.0f, 8.0f }, { 1.0f, 1.0f } });

	mHolder->setMargin(4.0f);

	mContent->setTexture(TEXTURE("textures/ui/progressbar_content.png"));
	mContent->setCenterRegion({ { 6.0f, 6.0f }, { 1.0f, 1.0f } });
}

// dollar emitter

DollarEmitter::DollarEmitter() : Shared::SceneHelpers::SpriteEmitter(Holder)
{
	setTexture(TEXTURE("textures/ui/dollar_particle.png"));
	setRunning(false);
	setBeginSize({ 0.0f, 0.0f });
	setPivot(0.5f);
	setAnchor(0.5f);
	setDistance(156.0f);
	setMinDuration(0.25f);
	setMaxDuration(0.75f);
}

void DollarEmitter::emitPack()
{
	emit(8);
}