#include "bottom_menu.h"

using namespace phoneinc;

BottomMenu::BottomMenu()
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/bottom_menu/background.png"));
	background->setAnchor({ 0.5f, 1.0f });
	background->setPivot({ 0.5f, 1.0f });
	background->setScale(1.0f / 3.0f);
	attach(background);

	auto video_button = std::make_shared<Scene::Sprite>();
	video_button->setTexture(TEXTURE("textures/bottom_menu/button.png"));
	video_button->setAnchor({ 0.5f, 0.0f });
	video_button->setPivot({ 0.5f, 0.0f });
	video_button->setY(-16.0f);
	background->attach(video_button);

	auto video_icon = std::make_shared<Scene::Sprite>();
	video_icon->setTexture(TEXTURE("textures/bottom_menu/video.png"));
	video_icon->setAnchor({ 0.0f, 0.5f });
	video_icon->setPivot({ 0.0f, 0.5f });
	video_icon->setPosition({ 24.0f, -12.0f });
	video_button->attach(video_icon);

	auto video_text = std::make_shared<Scene::Outlined<Scene::Label>>();
	video_text->setFont(FONT("default"));
	video_text->setText("awdawd");
	video_text->setPivot({ 0.0f, 0.5f });
	video_text->setAnchor({ 1.0f, 0.5f });
	video_text->setPosition({ 8.0f, 0.0f });
	video_text->setScale(3.0f);
	video_icon->attach(video_text);
}