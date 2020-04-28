#include "street.h"
#include "helpers.h"
#include "windows/warehouse_window.h"
#include "windows/shop_window.h"

using namespace PhoneInc;

Street::Street()
{
	setHeight(682.0f);

	auto city = std::make_shared<Scene::Sprite>();
	city->setTexture(TEXTURE("textures/street/city.png"));
	city->setPivot({ 0.5f, 0.0f });
	city->setAnchor({ 0.5f, 0.0f });
	attach(city);

	auto floor = std::make_shared<Scene::Sprite>();
	floor->setTexture(TEXTURE("textures/street/floor.png"));
	floor->setPivot({ 0.5f, 1.0f });
	floor->setAnchor({ 0.5f, 1.0f });
	attach(floor);

	auto warehouse = std::make_shared<Scene::Sprite>();
	warehouse->setTexture(TEXTURE("textures/warehouse/1.png"));
	warehouse->setPosition({ 8.0f, -60.0f });
	floor->attach(warehouse);

	auto warehouse_button = std::make_shared<Helpers::StandardButton>();
	warehouse_button->setPivot(0.5f);
	warehouse_button->setPosition({ 146.0f, 412.0f });
	warehouse_button->getLabel()->setText(LOCALIZE("UPGRADE_BUTTON"));
	warehouse_button->setClickCallback([this] {
		auto window = std::make_shared<WarehouseWindow>();
		EVENT->emit(Helpers::PushWindowEvent({ window }));
	});
	warehouse->attach(warehouse_button);

	auto shop = std::make_shared<Scene::Sprite>();
	shop->setTexture(TEXTURE("textures/shop/1.png"));
	shop->setPosition({ 841.0f, -60.0f });
	floor->attach(shop);

	auto shop_button = std::make_shared<Helpers::StandardButton>();
	shop_button->setPivot(0.5f);
	shop_button->setPosition({ 116.0f, 412.0f });
	shop_button->getLabel()->setText(LOCALIZE("UPGRADE_BUTTON"));
	shop_button->setClickCallback([] {
		auto window = std::make_shared<ShopWindow>();
		EVENT->emit(Helpers::PushWindowEvent({ window }));
	});
	shop->attach(shop_button);

	auto progressbar1 = std::make_shared<Helpers::Progressbar>();
	progressbar1->setPivot(0.5f);
	progressbar1->setPosition({ 164.0f, 62.0f });
	progressbar1->setSize({ 256.0f, 22.0f });
	progressbar1->setProgress(0.25f);
	attach(progressbar1);

	auto progressbar2 = std::make_shared<Helpers::Progressbar>();
	progressbar2->setPivot(0.5f);
	progressbar2->setPosition({ 928.0f, 62.0f });
	progressbar2->setSize({ 256.0f, 22.0f });
	progressbar2->setProgress(0.25f);
	attach(progressbar2);
}