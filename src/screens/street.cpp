#include "street.h"
#include "helpers.h"
#include "windows/warehouse_window.h"
#include "truck.h"
#include "balance.h"

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

	mWarehouse = std::make_shared<Scene::Sprite>();
	mWarehouse->setPosition({ 8.0f, -60.0f });
	floor->attach(mWarehouse);

	auto warehouse_button = std::make_shared<Helpers::StandardButton>();
	warehouse_button->setPivot(0.5f);
	warehouse_button->setPosition({ 146.0f, 412.0f });
	warehouse_button->getLabel()->setText(LOCALIZE("UPGRADE_BUTTON"));
	warehouse_button->setClickCallback([this] {
		auto window = std::make_shared<WarehouseWindow>();
		EVENT->emit(Helpers::PushWindowEvent({ window }));
	});
	mWarehouse->attach(warehouse_button);

	mWarehouseProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mWarehouseProgressbar->setPivot(0.5f);
	mWarehouseProgressbar->setPosition({ 164.0f, 62.0f });
	mWarehouseProgressbar->setSize({ 256.0f, 20.0f });
	mWarehouseProgressbar->setProgress(0.0f);
	attach(mWarehouseProgressbar);

	runAction(Shared::ActionHelpers::ExecuteInfinite([this] {
		if (mWarehouseBusy)
			return;

		if (PROFILE->getWarehouseStorage() == 0)
			return;

		mWarehouseBusy = true;
		runWarehouseAction();
	}));

	refresh();
}

void Street::refresh()
{
	mWarehouse->setTexture(TEXTURE(fmt::format("textures/warehouse/{}.png", Balance::GetWarehouseStage())));
}

void Street::event(const Profile::WarehouseLevelChangedEvent& e)
{
	refresh();
}

void Street::runWarehouseAction()
{
	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::Interpolate(0.0f, 1.0f, Balance::GetWarehouseDuration(), Common::Easing::Linear, [this](float value) {
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
	const float Dest = 1080.0f + 100.0f;

	auto truck = std::make_shared<Truck>();
	truck->setPivot(0.5f);
	truck->setPosition({ Start, 154.0f });
	mTruckHolder->attach(truck);

	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::ChangeHorizontalPosition(truck, Dest, 5.0f),
		Shared::ActionHelpers::Execute([this] {
			PROFILE->setCash(PROFILE->getCash() + 100.0);
		}),
		Shared::ActionHelpers::Kill(truck)
	));
}