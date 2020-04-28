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
	setScale(Scale);
}

LabelSolid::LabelSolid()
{
	setOutlineThickness(0.0f);
}

LabelSolidBold::LabelSolidBold()
{
	setFont(FONT("default_bold"));
}

Button::Button()
{
	setClickCallback([this] {
		auto executeCallback = [](auto callback) { if (callback) callback(); };
		if (isActive())
			executeCallback(mActiveCallback);
		else
			executeCallback(mInactiveCallback);
	});
	setChooseCallback([this] {
		recursiveColorSet(glm::vec4(1.25f), shared_from_this());
	});
	setCancelChooseCallback([this] {
		recursiveColorSet(glm::vec4(1.0f), shared_from_this());
	});
}

void Button::recursiveColorSet(const glm::vec4& value, std::shared_ptr<Scene::Node> node)
{
	for (auto child : node->getNodes())
	{
		recursiveColorSet(value, child);
	}

	auto color_node = std::dynamic_pointer_cast<Scene::Color>(node);

	if (!color_node)
		return;

	color_node->setColor(value);
}

void Button::setActive(bool value)
{
	mActive = value;
	ensureTexture();
}

void Button::setActiveTexture(std::shared_ptr<Renderer::Texture> value)
{
	mActiveTexture = value;
	ensureTexture();
}

void Button::setInactiveTexture(std::shared_ptr<Renderer::Texture> value)
{
	mInactiveTexture = value;
	ensureTexture();
}

void Button::ensureTexture()
{
	if (!mActiveTexture || !mInactiveTexture)
		return;

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
	mLabel->setFontSize(12.0f);
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
	mLabel->setFontSize(12.0f);
	mLabel->setY(-12.0f);
	attach(mLabel);
}

CloseButtonWidget::CloseButtonWidget() : Button()
{
	setTexture(TEXTURE("textures/buttons/close.png"));
	setClickCallback([] {
		EVENT->emit(PopWindowEvent());
	});
}

Progressbar::Progressbar()
{
	mHolder = std::make_shared<Scene::Node>();
	mHolder->setAnchor(0.5f);
	mHolder->setPivot(0.5f);
	mHolder->setStretch(1.0f);
	attach(mHolder);

	mClip = std::make_shared<Scene::Clippable<Scene::Node>>();
	mClip->setStretch(1.0f);
	mClip->setAnchor({ 0.0f, 0.5f });
	mClip->setPivot({ 0.0f, 0.5f });
	mHolder->attach(mClip);

	mContent = std::make_shared<Scene::SlicedSprite>();
	mContent->setStretch({ -1.0f, 1.0f });
	mContent->setAnchor({ 0.0f, 0.5f });
	mContent->setPivot({ 0.0f, 0.5f });
	runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
		mContent->setWidth(mHolder->getWidth());
	}));
	mClip->attach(mContent);
}

StreetProgressbar::StreetProgressbar() : Progressbar()
{
	setTexture(TEXTURE("textures/ui/progressbar_background.png"));
	setCenterRegion({ { 8.0f, 8.0f }, { 1.0f, 1.0f } });

	mHolder->setMargin(4.0f);

	mContent->setTexture(TEXTURE("textures/ui/progressbar_content.png"));
	mContent->setCenterRegion({ { 6.0f, 6.0f }, { 1.0f, 1.0f } });
}