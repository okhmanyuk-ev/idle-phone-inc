#pragma once

#include <shared/all.h>
#include "factory.h"

namespace PhoneInc
{
	class Factory::Room : public Scene::Sprite
	{
	public:
		Room(int level);
	};
}