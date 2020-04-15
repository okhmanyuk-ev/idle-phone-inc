#include "profile.h"

using namespace PhoneInc;

void Profile::load()
{
	auto path = PLATFORM->getAppFolder() + "save.bson";

	if (!Platform::Asset::Exists(path, Platform::Asset::Path::Absolute))
		return;

	auto json_file = Platform::Asset(path, Platform::Asset::Path::Absolute);
	auto json = nlohmann::json::from_bson(std::string((char*)json_file.getMemory(), json_file.getSize()));

	//if (json.contains("test"))
	//	mHighScore = json["test"];
}

void Profile::save()
{
	auto json = nlohmann::json();
	json["test"] = 123;
	auto bson = nlohmann::json::to_bson(json);
	Platform::Asset::Write(PLATFORM->getAppFolder() + "save.bson", bson.data(), bson.size(), Platform::Asset::Path::Absolute);
}

void Profile::clear()
{
	//mHighScore = 0;
}

void Profile::saveAsync()
{
	TASK->addTask([this] {
		save();
	});
}