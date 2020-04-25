#include "profile.h"

using namespace PhoneInc;

void Profile::load()
{
	auto path = PLATFORM->getAppFolder() + "save.bson";

	if (!Platform::Asset::Exists(path, Platform::Asset::Path::Absolute))
		return;

	auto json_file = Platform::Asset(path, Platform::Asset::Path::Absolute);
	auto json = nlohmann::json::from_bson(std::string((char*)json_file.getMemory(), json_file.getSize()));

	auto tryRead = [json](auto& src, auto name) {
		if (json.contains(name))
			src = json.at(name);
	};
	
	tryRead(mCash, "cash");

	if (json.contains("rooms"))
	{
		for (auto field : json.at("rooms").items())
		{
			auto index = std::stoi(field.key());
			auto value = field.value();

			auto tryReadValue = [value](auto& src, auto name) {
				if (value.contains(name))
					src = value.at(name);
			};
			
			auto room = Room();
			
			tryReadValue(room.product, "product");
			tryReadValue(room.manager, "manager");
			tryReadValue(room.worker1, "worker1");
			tryReadValue(room.worker2, "worker2");
			tryReadValue(room.worker3, "worker3");

			mRooms[index] = room;
		}
	}

	tryRead(mWarehouseLevel, "warehouse_level");
	tryRead(mShopLevel, "shop_level");
}

void Profile::save()
{
	mSaveMutex.lock();

	auto json = nlohmann::json();
	
	json["cash"] = mCash;

	for (const auto& [index, room] : mRooms)
	{
		auto& value = json["rooms"][index];
		value["product"] = room.product;
		value["manager"] = room.manager;
		value["worker1"] = room.worker1;
		value["worker2"] = room.worker2;
		value["worker3"] = room.worker3;
	}

	json["warehouse_level"] = mWarehouseLevel;
	json["shop_level"] = mShopLevel;

	auto bson = nlohmann::json::to_bson(json);
	Platform::Asset::Write(PLATFORM->getAppFolder() + "save.bson", bson.data(), bson.size(), Platform::Asset::Path::Absolute);

	mSaveMutex.unlock();
}

void Profile::clear()
{
	setCash(30.0);
	mRooms.clear();
	setWarehouseLevel(1);
	setShopLevel(1);
	EVENT->emit(ProfileClearedCallback());
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
	return mRooms.count(index) == 0;
}

void Profile::unlockRoom(int index)
{
	assert(isRoomLocked(index));
	mRooms.insert({ index, Room() });
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

void Profile::setRoom(int index, Room value)
{
	if (mRooms.count(index) > 0 && mRooms.at(index) == value)
		return;

	auto checkBounds = [](auto& src, auto maxValue) {
		assert(src >= 0 && src <= maxValue);
		
		src = glm::clamp(src, 0, maxValue);
	};

	checkBounds(value.product, Room::MaxProductLevel);
	checkBounds(value.manager, Room::MaxManagerLevel);
	checkBounds(value.worker1, Room::MaxWorkerLevel);
	checkBounds(value.worker2, Room::MaxWorkerLevel);
	checkBounds(value.worker3, Room::MaxWorkerLevel);

	mRooms[index] = value;
	EVENT->emit(RoomChangedEvent({ index }));
	saveAsync();
}