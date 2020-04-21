#include "profile.h"

using namespace PhoneInc;

void Profile::load()
{
	auto path = PLATFORM->getAppFolder() + "save.bson";

	if (!Platform::Asset::Exists(path, Platform::Asset::Path::Absolute))
		return;

	auto json_file = Platform::Asset(path, Platform::Asset::Path::Absolute);
	auto json = nlohmann::json::from_bson(std::string((char*)json_file.getMemory(), json_file.getSize()));


	auto readValue = [json](auto& src, auto name) {
		if (json.contains(name))
			src = json.at(name);
	};
	
	readValue(mCash, "cash");

	if (json.contains("unlocked_rooms"))
		mUnlockedRooms = json["unlocked_rooms"].get<std::set<int>>();

	readValue(mWarehouseLevel, "warehouse_level");
	readValue(mShopLevel, "shop_level");
}

void Profile::save()
{
	mSaveMutex.lock();

	auto json = nlohmann::json();
	
	json["cash"] = mCash;
	json["unlocked_rooms"] = mUnlockedRooms;
	json["warehouse_level"] = mWarehouseLevel;
	json["shop_level"] = mShopLevel;

	auto bson = nlohmann::json::to_bson(json);
	Platform::Asset::Write(PLATFORM->getAppFolder() + "save.bson", bson.data(), bson.size(), Platform::Asset::Path::Absolute);

	mSaveMutex.unlock();
}

void Profile::clear()
{
	setCash(10.0);
	setUnlockedRooms({ });
	setWarehouseLevel(1);
	setShopLevel(1);
}

void Profile::saveAsync()
{
	TASK->addTask([this] {
		save();
	});
}

bool Profile::isEnoughCash(double value) const
{
	return getCash() >= value;
}

void Profile::spendCash(double value)
{
	assert(isEnoughCash(value));
	setCash(getCash() - value);
	PROFILE->saveAsync();
}

bool Profile::isRoomLocked(int index) const
{
	return mUnlockedRooms.count(index) == 0;
}

void Profile::unlockRoom(int index)
{
	assert(isRoomLocked(index));
	mUnlockedRooms.insert(index);
	EVENT->emit(RoomUnlockedEvent({ index }));
	saveAsync();
}

void Profile::setCash(double value)
{
	if (mCash == value)
		return;

	mCash = value;
	EVENT->emit(CashChangedEvent());
	saveAsync();
}