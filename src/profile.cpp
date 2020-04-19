#include "profile.h"

using namespace PhoneInc;

void Profile::load()
{
	auto path = PLATFORM->getAppFolder() + "save.bson";

	if (!Platform::Asset::Exists(path, Platform::Asset::Path::Absolute))
		return;

	auto json_file = Platform::Asset(path, Platform::Asset::Path::Absolute);
	auto json = nlohmann::json::from_bson(std::string((char*)json_file.getMemory(), json_file.getSize()));

	if (json.contains("cash"))
		mCash = json["cash"];
}

void Profile::save()
{
	mSaveMutex.lock();

	auto json = nlohmann::json();
	json["cash"] = mCash;
	auto bson = nlohmann::json::to_bson(json);
	Platform::Asset::Write(PLATFORM->getAppFolder() + "save.bson", bson.data(), bson.size(), Platform::Asset::Path::Absolute);

	mSaveMutex.unlock();
}

void Profile::clear()
{
	setCash(10.0);
}

void Profile::saveAsync()
{
	TASK->addTask([this] {
		save();
	});
}

bool Profile::isEnoughCash(double value)
{
	return getCash() >= value;
}

void Profile::spendCash(double value)
{
	assert(isEnoughCash(value));
	setCash(getCash() - value);
	PROFILE->saveAsync();
}

void Profile::setCash(double value)
{
	if (mCash == value)
		return;

	mCash = value;
	EVENT->emit(CashChangedEvent());
}