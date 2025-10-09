#include "bottom_menu.h"
#include "helpers.h"
#include "windows/bottom_menu_locked_window.h"

using namespace PhoneInc;

BottomMenu::BottomMenu()
{
	setTouchable(true);
	setVerticalOrigin(-153.0f);

	auto asset = sky::Asset("xml/bottom_menu.xml");
	auto xml = std::string((const char*)asset.getMemory(), asset.getSize());
	auto [node, collection] = Shared::SceneHelpers::CreateNodesFromXml(xml);
	attach(node);

	for (auto name : { "video_button", "shop_icon", "boosters_icon", "upgrades_icon", "map_icon" })
	{
		auto button = std::static_pointer_cast<Helpers::Button>(collection.at(name));
		button->setClickCallback([] {
			SCENE_MANAGER->pushWindow(std::make_shared<BottomMenuLockedWindow>());
		});
	}
}