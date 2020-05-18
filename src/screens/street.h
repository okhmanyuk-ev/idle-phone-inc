#pragma once

#include <shared/all.h>
#include "helpers.h"

namespace PhoneInc
{
	class Street : public Scene::Actionable<Scene::Cullable<Scene::Node>>,
		public Common::EventSystem::Listenable<Profile::WarehouseLevelChangedEvent>,
		public Common::EventSystem::Listenable<Profile::WarehouseStorageChangeEvent>
	{
	public:
		class Truck;
	
	public:
		Street();

	public:
		void refresh();

	private:
		void event(const Profile::WarehouseLevelChangedEvent& e) override;
		void event(const Profile::WarehouseStorageChangeEvent& e) override;

	private:
		void runWarehouseAction();
		void runTruckAction();
		void refreshWarehouseStorageLabel();
		
	private:
		bool mWarehouseBusy = false;
		std::shared_ptr<Helpers::StreetProgressbar> mWarehouseProgressbar;
		std::shared_ptr<Helpers::Label> mWarehouseStorageLabel;
		std::shared_ptr<Scene::Node> mTruckHolder;
		std::shared_ptr<Scene::Sprite> mWarehouse;
	};
}