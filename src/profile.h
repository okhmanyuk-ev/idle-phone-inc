#pragma once

#include <shared/all.h>
#include <array>
#include "balance.h"

#define PROFILE ENGINE->getSystem<PhoneInc::Profile>()

namespace PhoneInc
{
	class Profile
	{
	public:
		struct ProfileClearedEvent { };
		struct CashChangedEvent { };
		struct RoomUnlockedEvent { int index; };
		struct RoomChangedEvent { int index; };
		struct WarehouseLevelChangedEvent { };
		struct WarehouseStorageChangeEvent { };

	public:
		struct Room
		{
			int product = 0; // 0 means doesnt opened
			int manager = 0;
			std::array<int, Balance::MaxWorkersCount> workers = { 0, 0, 0 };
		};

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
		
		const auto& getRooms() const { return mRooms; }
		void setRoom(int index, Room value);

		auto getWarehouseLevel() const { return mWarehouseLevel; }
		void setWarehouseLevel(int value);

		auto getWarehouseStorage() const { return mWarehouseStorage; }
		void setWarehouseStorage(double value);

		bool isNightBackground() const { return mNightBackground; }
		void setNightBackground(bool value) { mNightBackground = value; }

	private:
		double mCash = 0.0f;
		std::map<int, Room> mRooms = { };
		int mWarehouseLevel = 0;
		double mWarehouseStorage = 0;
		bool mNightBackground = true;
	};

	inline bool operator==(const Profile::Room& left, const Profile::Room& right)
	{
		return
			left.product == right.product &&
			left.manager == right.manager &&
			left.workers == right.workers;
	}

	inline bool operator!=(const Profile::Room& left, const Profile::Room& right)
	{
		return !(left == right);
	}
}
