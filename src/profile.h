#pragma once

#include <shared/all.h>

#define PROFILE ENGINE->getSystem<PhoneInc::Profile>()

namespace PhoneInc
{
	class Profile
	{
	public:
		void load();
		void save();
		void saveAsync();
		void clear();
	};
}
