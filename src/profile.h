#pragma once

#include <shared/all.h>
#include <array>
#include "balance.h"
#include "microtasks.h"

#define PROFILE ENGINE->getSystem<PhoneInc::Profile>()

namespace PhoneInc
{
	class Profile
	{
	public:
		struct ProfileClearedEvent { };
		struct CashChangedEvent { };
        struct CoinsChangedEvent { };
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
        
        auto getCoins() const { return mCoins; }
        void setCoins(double value);
		
		const auto& getRooms() const { return mRooms; }
		void setRoom(int index, Room value);

		auto getWarehouseLevel() const { return mWarehouseLevel; }
		void setWarehouseLevel(int value);

		auto getWarehouseStorage() const { return mWarehouseStorage; }
		void setWarehouseStorage(double value);

		bool isNightBackground() const { return mNightBackground; }
		void setNightBackground(bool value);

		bool isTutorCompleted(const std::string& name) { return mCompletedTutors.count(name) > 0; }
		void setTutorCompleted(const std::string& name);

		void setMicrotaskProgress(Microtasks::Task::Type type, int value);
		auto getMicrotaskProgress(Microtasks::Task::Type type) { return mMicrotaskProgresses[type]; }

		void setMicrotaskIndex(int value);
		auto getMicrotaskIndex() const { return mMicrotaskIndex; }

	private:
		double mCash = 0.0f;
        double mCoins = 0.0f;
		std::map<int, Room> mRooms = { };
		int mWarehouseLevel = 0;
		double mWarehouseStorage = 0;
		bool mNightBackground = true;
		std::set<std::string> mCompletedTutors = { };
		std::map<Microtasks::Task::Type, int> mMicrotaskProgresses;
		int mMicrotaskIndex = 0;
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
