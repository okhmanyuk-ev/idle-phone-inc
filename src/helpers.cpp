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