#include "bottom_menu.h"
#include "helpers.h"
#include "windows/bottom_menu_locked_window.h"

using namespace PhoneInc;

BottomMenu::BottomMenu()
{
	setTouchable(true);
	setVerticalOrigin(-153.0f);

	auto [node, collection] = Shared::SceneHelpers::CreateNodesFromXml(R"(
		<Sprite texture="textures/bottom_menu/background.png" batch_group="ui_menu">
			<MyButton id="video_button" anchor="0.5,0" pivot="0.5,0" y="-16" texture="textures/bottom_menu/button.png" batch_group="ui_menu_icon">
				<Sprite anchor="0,0.5" pivot="0,0.5" pos="24,-12" texture="textures/bottom_menu/video.png">
					<MyLabel anchor="1,0.5" pivot="0,0.5" x="8" font_size="42" text="BOTTOM_VIDEO_BUTTON" localized="true"/>
				</Sprite>
			</MyButton>
			<MyLabel anchor="0.5,0" pivot="0.5" pos="-454,98.5" font_size="30" text="BOTTOM_SHOP_BUTTON" localized="true">
				<MyButton id="shop_icon" anchor="0.5,0" pivot="0.5,1" y="-4" texture="textures/bottom_menu/shop.png" batch_group="ui_menu_icon"/>
			</MyLabel>
			<MyLabel anchor="0.5,0" pivot="0.5" pos="-257,98.5" font_size="30" text="BOTTOM_BOOSTERS_BUTTON" localized="true">
				<MyButton id="boosters_icon" anchor="0.5,0" pivot="0.5,1" y="-4" texture="textures/bottom_menu/boosters.png" batch_group="ui_menu_icon"/>
			</MyLabel>
			<MyLabel anchor="0.5,0" pivot="0.5" pos="264,98.5" font_size="30" text="BOTTOM_UPGRADES_BUTTON" localized="true">
				<MyButton id="upgrades_icon" anchor="0.5,0" pivot="0.5,1" y="-4" texture="textures/bottom_menu/upgrades.png" batch_group="ui_menu_icon"/>
			</MyLabel>
			<MyLabel anchor="0.5,0" pivot="0.5" pos="455,98.5" font_size="30" text="BOTTOM_MAP_BUTTON" localized="true">
				<MyButton id="map_icon" anchor="0.5,0" pivot="0.5,1" y="-4" texture="textures/bottom_menu/map.png" batch_group="ui_menu_icon"/>
			</MyLabel>
		</Sprite>
	)");
	attach(node);

	for (auto name : { "video_button", "shop_icon", "boosters_icon", "upgrades_icon", "map_icon" })
	{
		auto button = std::static_pointer_cast<Helpers::Button>(collection.at(name));
		button->setClickCallback([] {
			SCENE_MANAGER->pushWindow(std::make_shared<BottomMenuLockedWindow>());
		});
	}
}