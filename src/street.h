#pragma once

#include <shared/all.h>
#include "helpers.h"

namespace PhoneInc
{
	class Street : public Scene::Actionable<Scene::Cullable<Scene::Node>>
	{
	public:
		class Truck;
	
	public:
		Street();

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
	};
}