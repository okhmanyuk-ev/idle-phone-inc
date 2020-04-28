#pragma once

#include <shared/all.h>
#include "helpers.h"

namespace PhoneInc
{
	class Street : public Scene::Actionable<Scene::Cullable<Scene::Node>>
	{
	public:
		Street();

	private:
		void runWarehouseAction();
		void runTruckAction();
		
	private:
		bool mWarehouseBusy = false;
		std::shared_ptr<Helpers::StreetProgressbar> mWarehouseProgressbar;
		std::shared_ptr<Helpers::StreetProgressbar> mShopProgressbar;
	};
}