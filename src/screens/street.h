#pragma once

#include <shared/all.h>
#include "helpers.h"

namespace PhoneInc
{
	class Street : public Scene::Actionable<Scene::Cullable<Scene::Node>>,
		public Common::EventSystem::Listenable<Profile::WarehouseLevelChangedEvent>,
		public Common::EventSystem::Listenable<Profile::ShopLevelChangedEvent>
	{
	public:
		class Truck;
	
	public:
		Street();

	public:
		void refresh();

	private:
		void event(const Profile::WarehouseLevelChangedEvent& e) override;
		void event(const Profile::ShopLevelChangedEvent& e) override;

	private:
		void runWarehouseAction();
		void runTruckAction();
		void runShopAction();
		
	private:
		bool mWarehouseBusy = false;
		bool mShopBusy = false;
		std::shared_ptr<Helpers::StreetProgressbar> mWarehouseProgressbar;
		std::shared_ptr<Helpers::StreetProgressbar> mShopProgressbar;
		std::shared_ptr<Scene::Node> mTruckHolder;
		std::shared_ptr<Scene::Sprite> mWarehouse;
		std::shared_ptr<Scene::Sprite> mShop;
	};
}