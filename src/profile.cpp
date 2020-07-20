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
	nlohmann::json json;
	
	try
	{
		json = nlohmann::json::from_bson(std::string((char*)json_file.getMemory(), json_file.getSize()));
	}
	catch (const std::exception& e)
	{
		LOGC(e.what(), Console::Color::Red);
		LOGC("making new profile", Console::Color::Green);
		clear();
		return;
	}

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
	tryRead(mNightBackground, "night_background");

	if (json.contains("completed_tutors"))
		mCompletedTutors = json["completed_tutors"].get<std::set<std::string>>();

	if (json.contains("microtasks"))
	{
		for (auto field : json.at("microtasks").items())
		{
			auto type = Microtasks::StringToTaskType(field.key());
			int progress = field.value();

			mMicrotaskProgresses[type] = progress;
		}
	}

	tryRead(mMicrotaskIndex, "microtask_index");
}

void Profile::save()
{
	mSaveMutex.lock();

	auto json = nlohmann::json();
	
	json["cash"] = mCash;

	for (const auto& [index, room] : mRooms)
	{
		auto& entry = json["rooms"][index];
		entry["product"] = room.product;
		entry["manager"] = room.manager;
		entry["worker1"] = room.workers[0];
		entry["worker2"] = room.workers[1];
		entry["worker3"] = room.workers[2];
	}

	json["warehouse_level"] = mWarehouseLevel;
	json["night_background"] = mNightBackground;
	json["completed_tutors"] = mCompletedTutors;

	for (const auto& [type, progress] : mMicrotaskProgresses)
	{
		auto& entry = json["microtasks"][Microtasks::TaskTypeToString(type)];
		entry = progress;
	}

	json["microtask_index"] = mMicrotaskIndex;

	auto bson = nlohmann::json::to_bson(json);
	Platform::Asset::Write(PLATFORM->getAppFolder() + "save.bson", bson.data(), bson.size(), Platform::Asset::Path::Absolute);

	mSaveMutex.unlock();
}

void Profile::clear()
{
	EVENT->emit(ProfileClearedEvent());
	setCash(Balance::StartCash);
	mRooms.clear();
	setWarehouseLevel(1);
	setWarehouseStorage(0);
	mCompletedTutors.clear();
	mMicrotaskProgresses.clear();
	mMicrotaskIndex = 0;
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
	setMicrotaskProgress(Microtasks::Task::Type::UnlockRooms, mRooms.size());
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

	setMicrotaskProgress(Microtasks::Task::Type::ProductLevel, value.product);
	setMicrotaskProgress(Microtasks::Task::Type::ManagerLevel, value.manager);
	setMicrotaskProgress(Microtasks::Task::Type::WorkerLevel, value.workers[0]);
	setMicrotaskProgress(Microtasks::Task::Type::WorkerLevel, value.workers[1]);
	setMicrotaskProgress(Microtasks::Task::Type::WorkerLevel, value.workers[2]);

	saveAsync();
}

void Profile::setWarehouseLevel(int value)
{
	if (mWarehouseLevel == value)
		return;

	assert(value > 0);
	assert(value <= Balance::MaxWarehouseLevel);
	mWarehouseLevel = value;
	EVENT->emit(WarehouseLevelChangedEvent());
	setMicrotaskProgress(Microtasks::Task::Type::WarehouseLevel, value);
	saveAsync();
}

void Profile::setWarehouseStorage(double value)
{
	if (mWarehouseStorage == value)
		return;

	assert(value >= 0.0);
	mWarehouseStorage = value;
	EVENT->emit(WarehouseStorageChangeEvent());
	saveAsync();
}

void Profile::setNightBackground(bool value) {
	mNightBackground = value;
	saveAsync();
}

void Profile::setTutorCompleted(const std::string& name)
{
	mCompletedTutors.insert(name);
	saveAsync();
}

void Profile::setMicrotaskProgress(Microtasks::Task::Type type, int value)
{
	auto& progress = mMicrotaskProgresses[type];
	
	if (progress >= value)
		return;

	progress = value;
	MICROTASKS->checkForCompletion();
	saveAsync();
}

void Profile::setMicrotaskIndex(int value)
{
	mMicrotaskIndex = value;
	saveAsync();
}
