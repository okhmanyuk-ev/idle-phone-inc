#pragma once

#include "screen.h"

namespace phoneinc
{
	class Gameplay : public Scene::Actionable<Screen>
	{
	public:
		Gameplay();
	};
}