#pragma once

#include <shared/all.h>
#include "factory.h"

namespace PhoneInc
{
	enum class ManagerAnimation
	{
		Work,
		Idle
	};
	
	class Factory::Manager : public Scene::MappedAniSprite<ManagerAnimation>
	{
	public:
		Manager(int level);

	private:
		const static inline StateMap Animations = {
			{ Animation::Work, "work" },
			{ Animation::Idle, "idle" }
		};
	};
}