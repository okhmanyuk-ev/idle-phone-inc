#include "bottom_menu_locked_window.h"

using namespace PhoneInc;

BottomMenuLockedWindow::BottomMenuLockedWindow()
{
	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/windows/bottom_menu_locked_window/bg.png"));
	bg->setAnchor(0.5f);
	bg->setPivot(0.5f);
	getContent()->attach(bg);

	auto close_button = std::make_shared<Helpers::CloseButtonWidget>();
	close_button->setPosition({ 708.0f, 32.0f });
	bg->attach(close_button);

	auto description = std::make_shared<Helpers::Label>();
	description->setText(LOCALIZE("BOTTOM_MENU_LOCKED_WINDOW_DESCRIPTION"));
	description->setAnchor({ 0.5f, 1.0f });
	description->setPivot(0.5f);
	description->setPosition({ 0.0f, -182.0f });
	description->setFontSize(52.0f);
	description->setMultiline(true);
	description->setMultilineAlign(Graphics::TextMesh::Align::Center);
	description->setWidth(546.0f);
	bg->attach(description);
}