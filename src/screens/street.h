#pragma once

#include <sky/sky.h>
#include "helpers.h"

namespace PhoneInc
{
	class Street : public Scene::ClippableScissor<Scene::Cullable<Scene::Node>>,
		public sky::Listenable<Profile::WarehouseLevelChangedEvent>,
		public sky::Listenable<Profile::WarehouseStorageChangeEvent>,
		public sky::Listenable<Profile::CashChangedEvent>
	{
	public:
		class Truck;

	public:
		Street();

	public:
		void refresh();

	private:
		void onEvent(const Profile::WarehouseLevelChangedEvent& e) override;
		void onEvent(const Profile::WarehouseStorageChangeEvent& e) override;
		void onEvent(const Profile::CashChangedEvent& e) override;

	private:
		void runWarehouseAction();
		void runTruckAction();
		void refreshWarehouseStorageLabel();
		void refreshWarehouseButton();
		void spawnCloud();

	private:
		bool mWarehouseBusy = false;
		std::shared_ptr<Helpers::StreetProgressbar> mWarehouseProgressbar;
		std::shared_ptr<Helpers::Label> mWarehouseStorageLabel;
		std::shared_ptr<Scene::Node> mTruckHolder;
		std::shared_ptr<Scene::Sprite> mWarehouse;
		std::shared_ptr<Helpers::StandardButton> mWarehouseButton;
		std::shared_ptr<Scene::Node> mCloudsHolder;
	};
}