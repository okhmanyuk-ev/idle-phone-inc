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

	auto cash_label = std::make_shared<Helpers::LabelSolid>();
	cash_label->setAnchor({ 1.0f, 0.5f });
	cash_label->setPivot({ 1.0f, 0.5f });
	cash_label->setColor(Graphics::Color::ToNormalized(0, 255, 41));
	cash_label->setFontSize(39.0f);
	cash_label->setX(-64.0f);
	money_bg->attach(cash_label);

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

	auto coins_label = std::make_shared<Helpers::LabelSolid>();
	coins_label->setAnchor({ 1.0f, 0.5f });
	coins_label->setPivot({ 1.0f, 0.5f });
	coins_label->setColor(Graphics::Color::ToNormalized(255, 192, 24));
	coins_label->setFontSize(39.0f);
	coins_label->setX(-64.0f);
	coin_bg->attach(coins_label);
    
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

	runAction(Actions::Collection::ExecuteInfinite([coins_label, cash_label](auto dTime) {
		static double cash = 0.0;
		cash = Common::Helpers::SmoothValue(cash, PROFILE->getCash(), dTime);
		
		static double coins = 0.0;
		coins = Common::Helpers::SmoothValue(coins, PROFILE->getCoins(), dTime);
		
		cash_label->setText("$ " + Helpers::NumberToString(cash));
		coins_label->setText(Helpers::NumberToString(coins));
	}));
}
