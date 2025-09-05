#include "top_menu.h"
#include "helpers.h"

using namespace PhoneInc;

TopMenu::TopMenu()
{
	setTouchable(true);
	setVerticalOrigin(168.0f);

	auto [node, collection] = Shared::SceneHelpers::CreateNodesFromXml(R"(
		<Sprite texture="textures/top_menu/background.png" batch_group="ui_menu">
			<Sprite anchor="0,1" pivot="0.5" pos="454,-56" texture="textures/top_menu/money_bg.png" batch_group="ui_menu_background">
				<MyLabelSolid id="cash_label" anchor="1,0.5" pivot="1,0.5" x="-64" font_size="39" color="rgb(0,255,41) "/>
				<Sprite anchor="0,0.5" pivot="0.5" texture="textures/top_menu/money.png" batch_group="ui_menu_icon"/>
				<MyButton anchor="1,0.5" pivot="0.5" texture="textures/top_menu/add.png" batch_group="ui_menu_icon"/>
			</Sprite>
			<Sprite anchor="0,1" pivot="0.5" pos="900,-56" texture="textures/top_menu/coin_bg.png" batch_group="ui_menu_background">
				<MyLabelSolid id="coins_label" anchor="1,0.5" pivot="1,0.5" x="-64" font_size="39" color="rgb(255,192,24) "/>
				<Sprite anchor="0,0.5" pivot="0.5" texture="textures/top_menu/coin.png" batch_group="ui_menu_icon"/>
				<MyButton anchor="1,0.5" pivot="0.5" texture="textures/top_menu/add.png" batch_group="ui_menu_icon"/>
			</Sprite>
			<MyButton anchor="0,1" pivot="0.5" pos="67,-56" texture="textures/top_menu/settings.png" batch_group="ui_menu_icon"/>
		</Sprite>
	)");
	attach(node);

	auto cash_label = std::static_pointer_cast<Helpers::LabelSolid>(collection.at("cash_label"));
	auto coins_label = std::static_pointer_cast<Helpers::LabelSolid>(collection.at("coins_label"));

	runAction(sky::Actions::ExecuteInfinite([coins_label, cash_label](auto dTime) {
		static double cash = 0.0;
		cash = sky::ease_towards(cash, PROFILE->getCash(), dTime);

		static double coins = 0.0;
		coins = sky::ease_towards(coins, PROFILE->getCoins(), dTime);

		cash_label->setText(L"$ " + Helpers::NumberToString(cash));
		coins_label->setText(Helpers::NumberToString(coins));
	}));
}
