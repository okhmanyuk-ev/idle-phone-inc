#include "top_menu.h"
#include "helpers.h"

using namespace PhoneInc;

TopMenu::TopMenu()
{
	setBatchGroup("ui_menu");
	setTouchable(true);
	setTexture(TEXTURE("textures/top_menu/background.png"));
	setVerticalOrigin(168.0f);

	// money

	auto money_bg = std::make_shared<Scene::Sprite>();
	money_bg->setBatchGroup("ui_menu_background");
	money_bg->setTexture(TEXTURE("textures/top_menu/money_bg.png"));
	money_bg->setPivot(0.5f);
	money_bg->setAnchor({ 0.0f, 1.0f });
	money_bg->setPosition({ 454.0f, -56.0f });
	attach(money_bg);

	mCashLabel = std::make_shared<Helpers::LabelSolid>();
	mCashLabel->setAnchor({ 1.0f, 0.5f });
	mCashLabel->setPivot({ 1.0f, 0.5f });
	mCashLabel->setColor(Graphics::Color::ToNormalized(0, 255, 41));
	mCashLabel->setFontSize(39.0f);
	mCashLabel->setX(-64.0f);
	money_bg->attach(mCashLabel);
	refreshCashLabel();

	auto money_ico = std::make_shared<Scene::Sprite>();
	money_ico->setBatchGroup("ui_menu_icon");
	money_ico->setTexture(TEXTURE("textures/top_menu/money.png"));
	money_ico->setPivot(0.5f);
	money_ico->setAnchor({ 0.0f, 0.5f });
	money_bg->attach(money_ico);

	auto money_add_button = std::make_shared<Helpers::Button>();
	money_add_button->setBatchGroup("ui_menu_icon");
	money_add_button->setTexture(TEXTURE("textures/top_menu/add.png"));
	money_add_button->setPivot({ 0.5f, 0.5f });
	money_add_button->setAnchor({ 1.0f, 0.5f });
	money_bg->attach(money_add_button);

	// coins

	auto coin_bg = std::make_shared<Scene::Sprite>();
	coin_bg->setBatchGroup("ui_menu_background");
	coin_bg->setTexture(TEXTURE("textures/top_menu/coin_bg.png"));
	coin_bg->setPivot(0.5f);
	coin_bg->setAnchor({ 0.0f, 1.0f });
	coin_bg->setPosition({ 900.0f, -56.0f });
	attach(coin_bg);

	mCoinsLabel = std::make_shared<Helpers::LabelSolid>();
	mCoinsLabel->setAnchor({ 1.0f, 0.5f });
	mCoinsLabel->setPivot({ 1.0f, 0.5f });
	mCoinsLabel->setColor(Graphics::Color::ToNormalized(255, 192, 24));
	mCoinsLabel->setFontSize(39.0f);
	mCoinsLabel->setX(-64.0f);
	coin_bg->attach(mCoinsLabel);
    refreshCoinsLabel();
    
	auto coin_ico = std::make_shared<Scene::Sprite>();
	coin_ico->setBatchGroup("ui_menu_icon");
	coin_ico->setTexture(TEXTURE("textures/top_menu/coin.png"));
	coin_ico->setPivot(0.5f);
	coin_ico->setAnchor({ 0.0f, 0.5f });
	coin_bg->attach(coin_ico);

	auto coin_add_button = std::make_shared<Helpers::Button>();
	coin_add_button->setBatchGroup("ui_menu_icon");
	coin_add_button->setTexture(TEXTURE("textures/top_menu/add.png"));
	coin_add_button->setPivot({ 0.5f, 0.5f });
	coin_add_button->setAnchor({ 1.0f, 0.5f });
	coin_bg->attach(coin_add_button);

	auto settings_button = std::make_shared<Helpers::Button>();
	settings_button->setBatchGroup("ui_menu_icon");
	settings_button->setTexture(TEXTURE("textures/top_menu/settings.png"));
	settings_button->setPivot(0.5f);
	settings_button->setAnchor({ 0.0f, 1.0f });
	settings_button->setPosition({ 67.0f, -56.0f });
	attach(settings_button);
}

void TopMenu::onEvent(const Profile::CashChangedEvent& e)
{
    refreshCashLabel();
}

void TopMenu::onEvent(const Profile::CoinsChangedEvent& e)
{
    refreshCoinsLabel();
}

void TopMenu::refreshCashLabel()
{
	mCashLabel->setText("$ " + Helpers::NumberToString(PROFILE->getCash()));
}

void TopMenu::refreshCoinsLabel()
{
    mCoinsLabel->setText(Helpers::NumberToString(PROFILE->getCoins()));
}
