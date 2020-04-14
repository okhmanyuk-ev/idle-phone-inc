#include "gameplay.h"
#include "bottom_menu.h"

using namespace phoneinc;

Gameplay::Gameplay()
{
	auto background = std::make_shared<Scene::Sprite>();
	background->setTexture(TEXTURE("textures/main_background.png"));
	background->setStretch(1.0f);
	attach(background);

	auto bottom_menu = std::make_shared<BottomMenu>();
	bottom_menu->setAnchor({ 0.5f, 1.0f });
	bottom_menu->setPivot({ 0.5f, 1.0f });
	background->attach(bottom_menu);
}