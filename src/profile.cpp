#include "profile.h"

using namespace PhoneInc;

void Profile::load()
{
	auto path = PLATFORM->getAppFolder() + "save.bson";

	if (!Platform::Asset::Exists(path, Platform::Asset::Path::Absolute))
	{
		clear();
		return;
	}

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
			tryReadValue(room.workers[0], "worker1");
			tryReadValue(room.workers[1], "worker2");
			tryReadValue(room.workers[2], "worker3");

			mRooms[index] = room;
		}
	}

	tryRead(mWarehouseLevel, "warehouse_level");
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
		value["worker1"] = room.workers[0];
		value["worker2"] = room.workers[1];
		value["worker3"] = room.workers[2];
	}

	json["warehouse_level"] = mWarehouseLevel;

	auto bson = nlohmann::json::to_bson(json);
	Platform::Asset::Write(PLATFORM->getAppFolder() + "save.bson", bson.data(), bson.size(), Platform::Asset::Path::Absolute);

	mSaveMutex.unlock();
}

void Profile::clear()
{
	setCash(30.0);
	mRooms.clear();
	setWarehouseLevel(1);
	setWarehouseStorage(0);
	EVENT->emit(ProfileClearedEvent());
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

	checkBounds(value.product, Balance::MaxProductLevel);
	checkBounds(value.manager, Balance::MaxManagerLevel);
	for (auto& worker : value.workers)
	{
		checkBounds(worker, Balance::MaxWorkerLevel);
	}
	mRooms[index] = value;
	EVENT->emit(RoomChangedEvent({ index }));
	saveAsync();
}

void Profile::setWarehouseLevel(int value)
{
	assert(value > 0);
	assert(value <= Balance::MaxWarehouseLevel);
	mWarehouseLevel = value;
	EVENT->emit(WarehouseLevelChangedEvent());
	saveAsync();
}

void Profile::setWarehouseStorage(double value)
{
	assert(value >= 0.0);
	mWarehouseStorage = value;
}