#include "street.h"
#include "helpers.h"

using namespace PhoneInc;

Street::Street()
{
	setHeight(682.0f);

	auto city = std::make_shared<Scene::Sprite>();
	city->setTexture(TEXTURE("textures/city.png"));
	city->setPivot({ 0.5f, 0.0f });
	city->setAnchor({ 0.5f, 0.0f });
	attach(city);

	auto floor = std::make_shared<Scene::Sprite>();
	floor->setTexture(TEXTURE("textures/floor.png"));
	floor->setPivot({ 0.5f, 1.0f });
	floor->setAnchor({ 0.5f, 1.0f });
	attach(floor);

	auto warehouse = std::make_shared<Scene::Sprite>();
	warehouse->setTexture(TEXTURE("textures/warehouse/warehouse_1.png"));
	warehouse->setPosition({ 8.0f, -60.0f });
	floor->attach(warehouse);

	auto warehouse_button = std::make_shared<Helpers::Button>();
	warehouse_button->setPivot(0.5f);
	warehouse_button->setPosition({ 146.0f, 412.0f });
	warehouse_button->getLabel()->setText(LOCALIZE("ENHANCE_BUTTON"));
	warehouse->attach(warehouse_button);

	auto shop = std::make_shared<Scene::Sprite>();
	shop->setTexture(TEXTURE("textures/shop/shop_1.png"));
	shop->setPosition({ 841.0f, -60.0f });
	floor->attach(shop);

	auto shop_button = std::make_shared<Helpers::Button>();
	shop_button->setPivot(0.5f);
	shop_button->setPosition({ 116.0f, 412.0f });
	shop_button->getLabel()->setText(LOCALIZE("ENHANCE_BUTTON"));
	shop_button->setActive(false);
	shop->attach(shop_button);
}