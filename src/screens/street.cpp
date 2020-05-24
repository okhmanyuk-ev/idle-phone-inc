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

	if (PROFILE->isNightBackground())
		bg->setTexture(TEXTURE("textures/street/background_night.png"));
	else
		bg->setTexture(TEXTURE("textures/street/background.png"));

	bg->setPivot({ 0.5f, 0.0f });
	bg->setAnchor({ 0.5f, 0.0f });
	attach(bg);

	mTruckHolder = std::make_shared<Scene::Clippable<Scene::Node>>();
	mTruckHolder->setPosition({ 138.0f, 0.0f });
	mTruckHolder->setStretch(1.0f);
	mTruckHolder->setMargin({ mTruckHolder->getX(), 0.0f });
	bg->attach(mTruckHolder);

	mWarehouse = std::make_shared<Scene::Sprite>();
	mWarehouse->setPosition({ 24.0f, 574.0f });
	mWarehouse->setPivot({ 0.0f, 1.0f });
	bg->attach(mWarehouse);

	mWarehouseButton = std::make_shared<Helpers::StandardButton>();
	mWarehouseButton->setPivot(0.5f);
	mWarehouseButton->setPosition({ 966.0f, 538.0f });
	mWarehouseButton->getLabel()->setText(LOCALIZE("UPGRADE_BUTTON"));
	mWarehouseButton->setClickCallback([this] {
		auto window = std::make_shared<BuildingWindow>();
		EVENT->emit(Helpers::PushWindowEvent({ window }));
	});
	refreshWarehouseButton();
	attach(mWarehouseButton);

	mWarehouseProgressbar = std::make_shared<Helpers::StreetProgressbar>();
	mWarehouseProgressbar->setPivot(0.5f);
	mWarehouseProgressbar->setAnchor({ 0.5f, 0.0f });
	mWarehouseProgressbar->setPosition({ 0.0f, -64.0f });
	mWarehouseProgressbar->setSize({ 256.0f, 20.0f });
	mWarehouseProgressbar->setProgress(0.0f);
	mWarehouseProgressbar->setEnabled(false);
	mWarehouseProgressbar->setScale(0.0f);
	mWarehouse->attach(mWarehouseProgressbar);

	auto storage_bg = std::make_shared<Scene::Sprite>();
	storage_bg->setTexture(TEXTURE("textures/street/storage_bg.png"));
	storage_bg->setAnchor({ 0.5f, 0.0f });
	storage_bg->setPivot(0.5f);
	storage_bg->setPosition({ 0.0f, -154.0f });
	mWarehouse->attach(storage_bg);

	mWarehouseStorageLabel = std::make_shared<Helpers::Label>();
	mWarehouseStorageLabel->setAnchor(0.5f);
	mWarehouseStorageLabel->setPivot(0.5f);
	mWarehouseStorageLabel->setFontSize(12.0f);
	storage_bg->attach(mWarehouseStorageLabel);
	refreshWarehouseStorageLabel();

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

void Street::event(const Profile::WarehouseStorageChangeEvent& e)
{
	refreshWarehouseStorageLabel();
}

void Street::event(const Profile::CashChangedEvent& e)
{
	refreshWarehouseButton();
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
			mWarehouseProgressbar->setProgress(0.0f);
			mWarehouseProgressbar->setEnabled(false);
			mWarehouseBusy = false;
			runTruckAction();
		})
	));
}

void Street::runTruckAction()
{
	auto truck = std::make_shared<Truck>();
	STYLEBOOK->apply(truck, fmt::format("truck_{}", Balance::GetTruckStage()));
	mTruckHolder->attach(truck);

	auto storage = PROFILE->getWarehouseStorage();
	auto capacity = Balance::GetWarehouseTruckCapacity();
	
	if (capacity > storage)
		capacity = storage;

	PROFILE->setWarehouseStorage(storage - capacity);

	auto earning = Balance::GetWarehouseEarning() * capacity;
	truck->setEarning(earning); // TODO: earning
	
	float duration = Balance::GetWarehouseTruckDuration();

	runAction(Shared::ActionHelpers::MakeSequence(
		Shared::ActionHelpers::MakeParallel(
			Shared::ActionHelpers::ChangeHorizontalAnchor(truck, 1.0f, duration),
			Shared::ActionHelpers::ChangeHorizontalPivot(truck, 0.0f, duration)
		),
		Shared::ActionHelpers::Execute([truck] {
			PROFILE->setCash(PROFILE->getCash() + truck->getEarning());
		}),
		Shared::ActionHelpers::Kill(truck)
	));
}

void Street::refreshWarehouseStorageLabel()
{
	mWarehouseStorageLabel->setText(Helpers::NumberToString(PROFILE->getWarehouseStorage()));
}

void Street::refreshWarehouseButton()
{
	mWarehouseButton->setActive(BuildingWindow::CanUpgrade());
}
