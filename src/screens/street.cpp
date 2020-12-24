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

	mCloudsHolder = std::make_shared<Scene::Node>();
	mCloudsHolder->setStretch(1.0f);
	bg->attach(mCloudsHolder);

	mTruckHolder = std::make_shared<Scene::ClippableScissor<Scene::Node>>();
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
		SCENE_MANAGER->pushWindow(std::make_shared<BuildingWindow>());
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
	mWarehouseStorageLabel->setFontSize(36.0f);
	storage_bg->attach(mWarehouseStorageLabel);
	refreshWarehouseStorageLabel();

	runAction(Actions::Factory::ExecuteInfinite([this] {
		if (mWarehouseBusy)
			return;

		if (PROFILE->getWarehouseStorage() == 0)
			return;

		mWarehouseBusy = true;
		runWarehouseAction();
	}));

	refresh();

	// cloud spawner

	runAction(Actions::Factory::RepeatInfinite([this] {
		auto delay = glm::linearRand(4.0f, 8.0f);
		return Actions::Factory::Delayed(delay, Actions::Factory::Execute([this] {
			spawnCloud();
		}));
	}));
}

void Street::refresh()
{
	mWarehouse->setTexture(TEXTURE(fmt::format("textures/warehouse/{}.png", Balance::GetWarehouseStage())));
	mWarehouse->applyTextureSize();
}

void Street::onEvent(const Profile::WarehouseLevelChangedEvent& e)
{
	refresh();
}

void Street::onEvent(const Profile::WarehouseStorageChangeEvent& e)
{
	refreshWarehouseStorageLabel();
}

void Street::onEvent(const Profile::CashChangedEvent& e)
{
	refreshWarehouseButton();
}

void Street::runWarehouseAction()
{
	runAction(Actions::Factory::MakeSequence(
		Actions::Factory::Execute([this] {
			mWarehouseProgressbar->setEnabled(true);
		}),
		Actions::Factory::ChangeScale(mWarehouseProgressbar, { 1.0f, 1.0f }, 0.125f, Easing::BackOut),
			Actions::Factory::Interpolate(0.0f, 1.0f, Balance::GetWarehouseDuration(), Easing::Linear, [this](float value) {
			mWarehouseProgressbar->setProgress(value);
		}),
		Actions::Factory::ChangeScale(mWarehouseProgressbar, { 0.0f, 0.0f }, 0.125f, Easing::BackIn),
		Actions::Factory::Execute([this] {
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
	truck->setEarning(earning);
	
	float duration = Balance::GetWarehouseTruckDuration();

	runAction(Actions::Factory::MakeSequence(
		Actions::Factory::MakeParallel(
			Actions::Factory::ChangeHorizontalAnchor(truck, 1.0f, duration),
			Actions::Factory::ChangeHorizontalPivot(truck, 0.0f, duration)
		),
		Actions::Factory::Execute([truck] {
			PROFILE->setCash(PROFILE->getCash() + truck->getEarning());
		}),
		Actions::Factory::Kill(truck)
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

void Street::spawnCloud()
{
	auto texture = TEXTURE(Helpers::GetRandomElement<std::string>({
		"textures/street/cloud_1.png",
		"textures/street/cloud_2.png"
	}));

	float y = glm::linearRand(24.0f, 142.0f);

	auto cloud = std::make_shared<Scene::Actionable<Scene::Sprite>>();
	cloud->setTexture(texture);
	cloud->setAnchor(0.0f);
	cloud->setPivot({ 1.0f, 0.0f });
	cloud->setPosition({ 0.0f, y });
	mCloudsHolder->attach(cloud);

	auto duration = glm::linearRand(17.0f, 22.0f);

	cloud->runAction(Actions::Factory::MakeSequence(
		Actions::Factory::MakeParallel(
			Actions::Factory::ChangeHorizontalAnchor(cloud, 0.0f, 1.0f, duration),
			Actions::Factory::ChangeHorizontalPivot(cloud, 1.0f, 0.0f, duration)
		),
		Actions::Factory::Kill(cloud)
	));
}