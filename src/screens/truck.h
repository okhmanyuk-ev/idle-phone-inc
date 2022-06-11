#pragma once

#include <shared/all.h>
#include "street.h"

namespace PhoneInc
{
	enum class TruckAnimation
	{
		Main
	};
	
	class Street::Truck : public Scene::MappedAniSprite<TruckAnimation>
	{
	public:
		Truck();

	private:
		const static inline StateMap Animations = {
			{ Animation::Main, "main" }
		};

	public:
		auto getEarning() const { return mEarning; }
		void setEarning(double value) { mEarning = value; }

	private:
		double mEarning = 0.0f;
	};
}