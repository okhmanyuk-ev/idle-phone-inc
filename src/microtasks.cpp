#include "microtasks.h"
#include "profile.h"

using namespace PhoneInc;

std::string Microtasks::TaskTypeToString(Task::Type type)
{
	if (type == Task::Type::UnlockRooms)
		return "unlock_rooms";
	else if (type == Task::Type::ManagerLevel)
		return "manager_level";
	else if (type == Task::Type::WorkerLevel)
		return "worker_level";
	else if (type == Task::Type::WarehouseLevel)
		return "warehouse_level";
	else if (type == Task::Type::ProductLevel)
		return "product_level";
	
	assert(false);
	return "unknown";
}

Microtasks::Task::Type Microtasks::StringToTaskType(const std::string& name)
{
	if (name == "unlock_rooms")
		return Task::Type::UnlockRooms;
	else if (name == "manager_level")
		return Task::Type::ManagerLevel;
	else if (name == "worker_level")
		return Task::Type::WorkerLevel;
	else if (name == "warehouse_level")
		return Task::Type::WarehouseLevel;
	else if (name == "product_level")
		return Task::Type::ProductLevel;
	
	assert(false);
	return Task::Type::None;
}

Microtasks::Microtasks()
{
	auto json_file = Platform::Asset("microtasks.json");
	auto json = nlohmann::json::parse(std::string((char*)json_file.getMemory(), json_file.getSize()));
	
	for (auto entry : json.at("microtasks"))
	{
		Task task;
		task.type = StringToTaskType(entry.at("type"));
		task.target = entry.at("target");
		task.reward = entry.at("reward");
		task.locale = entry.at("locale");
		mTasks.push_back(task);
	}
}

void Microtasks::checkForCompletion()
{
	if (mReady)
		return;

	if (!hasUnfinishedTasks())
		return;

	const auto& task = getCurrentTask();
	auto progress = PROFILE->getMicrotaskProgress(task.type);
	
	if (progress < task.target)
		return;

	mReady = true;
	EVENT->emit(TaskReadyEvent());
}

const Microtasks::Task& Microtasks::getCurrentTask() const
{
	assert(hasUnfinishedTasks());
	auto index = PROFILE->getMicrotaskIndex();
	return mTasks.at(index);
}

bool Microtasks::hasUnfinishedTasks() const
{
	return PROFILE->getMicrotaskIndex() < mTasks.size();
}

void Microtasks::complete()
{
	PROFILE->setMicrotaskIndex(PROFILE->getMicrotaskIndex() + 1);
	mReady = false;
	EVENT->emit(TaskCompletedEvent());
}

void Microtasks::clear()
{
	mReady = false;
}
