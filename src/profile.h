#pragma once

#include <shared/all.h>

#define PROFILE ENGINE->getSystem<PhoneInc::Profile>()

namespace PhoneInc
{
	class Profile
	{
	public:
		struct CashChangedEvent { };
		struct RoomUnlockedEvent { int index; };

	public:
		void load();
		void save();
		void saveAsync();
		void clear();

	private:
		std::mutex mSaveMutex;

	public:
		bool isEnoughCash(double value) const;
		void spendCash(double value);
		
		bool isRoomLocked(int index) const;
		void unlockRoom(int index);

	public:
		auto getCash() const { return mCash; }
		void setCash(double value);
		
		auto getUnlockedRooms() const { return mUnlockedRooms; }
		void setUnlockedRooms(const std::set<int>& value) { mUnlockedRooms = value; }

		auto getWarehouseLevel() const { return mWarehouseLevel; }
		void setWarehouseLevel(int value) { mWarehouseLevel = value; }

		auto getShopLevel() const { return mShopLevel; }
		void setShopLevel(int value) { mShopLevel = value; }

	private:
		double mCash = 0.0;
		std::set<int> mUnlockedRooms = { };
		int mWarehouseLevel = 1;
		int mShopLevel = 1;
	};
}
