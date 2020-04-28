#include "street.h"
#include "helpers.h"
#include "windows/warehouse_window.h"
#include "windows/shop_window.h"
#include "truck.h"

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

	mTruckHolder = std::make_shared<Scene::Node>();
	floor->attach(mTruckHolder);

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

	mWarehouseProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mWarehouseProgressbar->setPivot(0.5f);
	mWarehouseProgressbar->setPosition({ 164.0f, 62.0f });
	mWarehouseProgressbar->setSize({ 256.0f, 22.0f });
	mWarehouseProgressbar->setProgress(0.0f);
	attach(mWarehouseProgressbar);

	mShopProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mShopProgressbar->setPivot(0.5f);
	mShopProgressbar->setPosition({ 928.0f, 62.0f });
	mShopProgressbar->setSize({ 256.0f, 22.0f });
	mShopProgressbar->setProgress(0.0f);
	attach(mShopProgressbar);

	runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
		if (mWarehouseBusy)
			return;

		if (PROFILE->getWarehouseStorage() == 0)
			return;

		mWarehouseBusy = true;
		runWarehouseAction();
	}));

	runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
		if (mShopBusy)
			return;

		if (PROFILE->getShopStorage() == 0)
			return;

		mShopBusy = true;
		runShopAction();
	}));

}

void Street::runWarehouseAction()
{
	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::Interpolate(0.0f, 1.0f, 5.0f, Common::Easing::Linear, [this](float value) {
			mWarehouseProgressbar->setProgress(value);
		}),
		Shared::ActionHelpers::Execute([this] {
			PROFILE->setWarehouseStorage(PROFILE->getWarehouseStorage() - 1);
			mWarehouseProgressbar->setProgress(0.0f);
			mWarehouseBusy = false;
			runTruckAction();
		})
	));
}


void Street::runTruckAction()
{
	const float Start = 148.0f;
	const float Dest = 934.0f;

	auto truck = std::make_shared<Truck>();
	truck->setPivot(0.5f);
	truck->setPosition({ Start, 154.0f });
	mTruckHolder->attach(truck);

	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::ChangeHorizontalPosition(truck, Dest, 5.0f),
		Shared::ActionHelpers::Kill(truck),
		Shared::ActionHelpers::Execute([this] {
			PROFILE->increaseShopStorage();
		})
	));
}

void Street::runShopAction()
{
	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::Interpolate(0.0f, 1.0f, 6.0f, Common::Easing::Linear, [this](float value) {
			mShopProgressbar->setProgress(value);
		}),
		Shared::ActionHelpers::Execute([this] {
			mShopProgressbar->setProgress(0.0f);
			PROFILE->setShopStorage(PROFILE->getShopStorage() - 1);
			mShopBusy = false;
			PROFILE->setCash(PROFILE->getCash() + 100.0);
		})
	));
}