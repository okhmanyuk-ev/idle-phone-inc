#pragma once

#include <shared/all.h>

#define PROFILE ENGINE->getSystem<PhoneInc::Profile>()

namespace PhoneInc
{
	class Profile
	{
	public:
		struct CashChangedEvent { };

	public:
		void load();
		void save();
		void saveAsync();
		void clear();

	private:
		std::mutex mSaveMutex;

	public:
		bool isEnoughCash(double value);
		void spendCash(double value);
		
	public:
		auto getCash() const { return mCash; }
		void setCash(double value);
		
	private:
		double mCash = 0.0;
	};
}
