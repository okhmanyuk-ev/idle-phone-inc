#pragma once

#include <shared/all.h>
#include "helpers.h"

namespace PhoneInc
{
	class Street : public Scene::Actionable<Scene::Cullable<Scene::Node>>,
		public Common::EventSystem::Listenable<Profile::WarehouseLevelChangedEvent>
	{
	public:
		class Truck;
	
	public:
		Street();

	public:
		void refresh();

	private:
		void event(const Profile::WarehouseLevelChangedEvent& e) override;

	private:
		void runWarehouseAction();
		void runTruckAction();
		
	private:
		bool mWarehouseBusy = false;
		std::shared_ptr<Helpers::StreetProgressbar> mWarehouseProgressbar;
		std::shared_ptr<Scene::Node> mTruckHolder;
		std::shared_ptr<Scene::Sprite> mWarehouse;
	};
}