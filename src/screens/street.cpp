#include "street.h"
#include "helpers.h"
#include "windows/building_window.h"
#include "truck.h"
#include "balance.h"

using namespace PhoneInc;

Street::Street()
{
	setHeight(670.0f);

	auto bg = std::make_shared<Scene::Sprite>();
	bg->setTexture(TEXTURE("textures/street/background.png"));
	bg->setPivot({ 0.5f, 0.0f });
	bg->setAnchor({ 0.5f, 0.0f });
	attach(bg);

	mTruckHolder = std::make_shared<Scene::Node>();
	bg->attach(mTruckHolder);

	mWarehouse = std::make_shared<Scene::Sprite>();
	mWarehouse->setPosition({ 24.0f, 574.0f });
	mWarehouse->setPivot({ 0.0f, 1.0f });
	bg->attach(mWarehouse);

	auto warehouse_button = std::make_shared<Helpers::StandardButton>();
	warehouse_button->setPivot(0.5f);
	warehouse_button->setPosition({ 966.0f, 538.0f });
	warehouse_button->getLabel()->setText(LOCALIZE("UPGRADE_BUTTON"));
	warehouse_button->setClickCallback([this] {
		auto window = std::make_shared<BuildingWindow>();
		EVENT->emit(Helpers::PushWindowEvent({ window }));
	}); 
	attach(warehouse_button);

	mWarehouseProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mWarehouseProgressbar->setPivot(0.5f);
	mWarehouseProgressbar->setPosition({ 164.0f, 194.0f });
	mWarehouseProgressbar->setSize({ 256.0f, 20.0f });
	mWarehouseProgressbar->setProgress(0.0f);
	mWarehouseProgressbar->setEnabled(false);
	mWarehouseProgressbar->setScale(0.0f);
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
	mWarehouse->setSize(0.0f);
}

void Street::event(const Profile::WarehouseLevelChangedEvent& e)
{
	refresh();
}

void Street::runWarehouseAction()
{
	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::Execute([this] {
			mWarehouseProgressbar->setEnabled(true);
		}),
		Shared::ActionHelpers::ChangeScale(mWarehouseProgressbar, { 1.0f, 1.0f }, 0.125f, Common::Easing::BackOut),
		Shared::ActionHelpers::Interpolate(0.0f, 1.0f, Balance::GetWarehouseDuration(), Common::Easing::Linear, [this](float value) {
			mWarehouseProgressbar->setProgress(value);
		}),
		Shared::ActionHelpers::ChangeScale(mWarehouseProgressbar, { 0.0f, 0.0f }, 0.125f, Common::Easing::BackIn),
		Shared::ActionHelpers::Execute([this] {
			PROFILE->setWarehouseStorage(PROFILE->getWarehouseStorage() - 1);
			mWarehouseProgressbar->setProgress(0.0f);
			mWarehouseProgressbar->setEnabled(false);
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
	truck->setPosition({ Start, 484.0f });
	mTruckHolder->attach(truck);

	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::ChangeHorizontalPosition(truck, Dest, 5.0f),
		Shared::ActionHelpers::Execute([this] {
			PROFILE->setCash(PROFILE->getCash() + 100.0);
		}),
		Shared::ActionHelpers::Kill(truck)
	));
}