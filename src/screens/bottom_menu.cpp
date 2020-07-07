#include "bottom_menu.h"
#include "helpers.h"

using namespace PhoneInc;

BottomMenu::BottomMenu()
{
	setTouchable(true);
	setTexture(TEXTURE("textures/bottom_menu/background.png"));
	setVerticalOrigin(-153.0f);

	// video

	auto video_button = std::make_shared<Helpers::Button>();
	video_button->setTexture(TEXTURE("textures/bottom_menu/button.png"));
	video_button->setAnchor({ 0.5f, 0.0f });
	video_button->setPivot({ 0.5f, 0.0f });
	video_button->setY(-16.0f);
	attach(video_button);

	auto video_icon = std::make_shared<Scene::Sprite>();
	video_icon->setTexture(TEXTURE("textures/bottom_menu/video.png"));
	video_icon->setAnchor({ 0.0f, 0.5f });
	video_icon->setPivot({ 0.0f, 0.5f });
	video_icon->setPosition({ 24.0f, -12.0f });
	video_button->attach(video_icon);

	auto video_text = std::make_shared<Helpers::Label>();
	video_text->setText(LOCALIZE("BOTTOM_VIDEO_BUTTON"));
	video_text->setPivot({ 0.0f, 0.5f });
	video_text->setAnchor({ 1.0f, 0.5f });
	video_text->setPosition({ 8.0f, 0.0f });
	video_text->setFontSize(42.0f);
	video_icon->attach(video_text);

	// shop

	auto shop_icon = std::make_shared<Scene::Sprite>();
	shop_icon->setTexture(TEXTURE("textures/bottom_menu/shop.png"));
	shop_icon->setAnchor({ 0.5f, 0.0f });
	shop_icon->setPivot(0.5f);
	shop_icon->setPosition({ -454.0f, -32.0f + 63.5 });
	attach(shop_icon);

	auto shop_text = std::make_shared<Helpers::Label>();
	shop_text->setText(LOCALIZE("BOTTOM_SHOP_BUTTON"));
	shop_text->setPivot(0.5f);
	shop_text->setAnchor({ 0.5f, 0.0f });
	shop_text->setPosition({ -454.0f, 35.0f + 63.5 });
	shop_text->setFontSize(30.0f);
	attach(shop_text);

	// boosters

	auto boosters_icon = std::make_shared<Scene::Sprite>();
	boosters_icon->setTexture(TEXTURE("textures/bottom_menu/boosters.png"));
	boosters_icon->setAnchor({ 0.5f, 0.0f });
	boosters_icon->setPivot(0.5f);
	boosters_icon->setPosition({ -257.0f, -28.0f + 63.5 });
	attach(boosters_icon);

	auto boosters_text = std::make_shared<Helpers::Label>();
	boosters_text->setText(LOCALIZE("BOTTOM_BOOSTERS_BUTTON"));
	boosters_text->setPivot(0.5f);
	boosters_text->setAnchor({ 0.5f, 0.0f });
	boosters_text->setPosition({ -257.0f, 35.0f + 63.5 });
	boosters_text->setFontSize(30.0f);
	attach(boosters_text);

	// upgrades

	auto upgrades_icon = std::make_shared<Scene::Sprite>();
	upgrades_icon->setTexture(TEXTURE("textures/bottom_menu/upgrades.png"));
	upgrades_icon->setAnchor({ 0.5f, 0.0f });
	upgrades_icon->setPivot(0.5f);
	upgrades_icon->setPosition({ 264.0f, -29.0f + 63.5 });
	attach(upgrades_icon);

	auto upgrades_text = std::make_shared<Helpers::Label>();
	upgrades_text->setText(LOCALIZE("BOTTOM_UPGRADES_BUTTON"));
	upgrades_text->setPivot(0.5f);
	upgrades_text->setAnchor({ 0.5f, 0.0f });
	upgrades_text->setPosition({ 264.0f, 35.0f + 63.5 });
	upgrades_text->setFontSize(30.0f);
	attach(upgrades_text);

	// map

	auto map_icon = std::make_shared<Scene::Sprite>();
	map_icon->setTexture(TEXTURE("textures/bottom_menu/map.png"));
	map_icon->setAnchor({ 0.5f, 0.0f });
	map_icon->setPivot(0.5f);
	map_icon->setPosition({ 455.0f, -29.0f + 63.5 });
	attach(map_icon);

	auto map_text = std::make_shared<Helpers::Label>();
	map_text->setText(LOCALIZE("BOTTOM_MAP_BUTTON"));
	map_text->setPivot(0.5f);
	map_text->setAnchor({ 0.5f, 0.0f });
	map_text->setPosition({ 455.0f, 35.0f + 63.5 });
	map_text->setFontSize(30.0f);
	attach(map_text);
}