#pragma once

#include <shared/all.h>

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

	public:
		struct Room
		{
			int product = 0; // 0 means doesnt opened
			int manager = 0;
			int worker1 = 0;
			int worker2 = 0;
			int worker3 = 0;
		};

	public:
		void load();
		void save();
		void saveAsync();
		void clear(bool emit = true);

	private:
		std::mutex mSaveMutex;

	public:
		bool isEnoughCash(double value) const;
		void spendCash(double value);
		
		bool isRoomLocked(int index) const;
		void unlockRoom(int index);

		bool isWarehouseFilled() const;
		void increaseWarehouseStorage();

		bool isShopFilled() const;
		void increaseShopStorage();
		
	public:
		auto getCash() const { return mCash; }
		void setCash(double value);
		
		const auto& getRooms() const { return mRooms; }
		void setRoom(int index, Room value);

		auto getWarehouseLevel() const { return mWarehouseLevel; }
		void setWarehouseLevel(int value) { mWarehouseLevel = value; }

		auto getShopLevel() const { return mShopLevel; }
		void setShopLevel(int value) { mShopLevel = value; }

		auto getWarehouseStorage() const { return mWarehouseStorage; }
		void setWarehouseStorage(int value);

		auto getShopStorage() const { return mShopStorage; }
		void setShopStorage(int value);

	private:
		double mCash = 0.0f;
		std::map<int, Room> mRooms = { };
		int mWarehouseLevel = 0;
		int mShopLevel = 0;
		int mWarehouseStorage = 0;
		int mShopStorage = 0;
	};

	inline bool operator==(const Profile::Room& left, const Profile::Room& right)
	{
		return
			left.product == right.product &&
			left.manager == right.manager &&
			left.worker1 == right.worker1 &&
			left.worker2 == right.worker2 &&
			left.worker3 == right.worker3;
	}

	inline bool operator!=(const Profile::Room& left, const Profile::Room& right)
	{
		return !(left == right);
	}
}
