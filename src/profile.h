#pragma once

#include <shared/all.h>

#define PROFILE ENGINE->getSystem<phoneinc::Profile>()

namespace phoneinc
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
