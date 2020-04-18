#pragma once

#include <shared/all.h>
#include "factory.h"

namespace PhoneInc
{
	enum class WorkerAnimation
	{
		Working
	};
	
	class Factory::Worker : public Scene::MappedAniSprite<WorkerAnimation>
	{
	public:
		Worker(int level);

	private:
		const static inline StateMap Animations = {
			{ Animation::Working, "working" }
		};
	};
}