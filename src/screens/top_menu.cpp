#include "top_menu.h"
#include "helpers.h"

using namespace PhoneInc;

TopMenu::TopMenu()
{
	setTouchable(true);
	setVerticalOrigin(168.0f);

	auto asset = sky::Asset("xml/top_menu.xml");
	auto xml = std::string((const char*)asset.getMemory(), asset.getSize());
	auto [node, collection] = Shared::SceneHelpers::CreateNodesFromXml(xml);
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
