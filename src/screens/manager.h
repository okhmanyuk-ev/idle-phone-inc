#pragma once

#include <shared/all.h>
#include "factory.h"
#include "helpers.h"

namespace PhoneInc
{
	enum class ManagerAnimation
	{
		Working,
		Idle
	};
	
	class Factory::Manager : public Scene::MappedAniSprite<ManagerAnimation>
	{
	public:
		Manager();

	public:
		void refresh();

	public:
		auto getLevel() const { return mLevel; }
		void setLevel(int value);

	private:
		int mLevel = 0;

	private:
		const static inline StateMap Animations = {
			{ Animation::Working, "working" },
			{ Animation::Idle, "idle" }
		};

	public:
		auto getProgressbar() { return mProgressbar; }

	private:
		std::shared_ptr<Helpers::StreetProgressbar> mProgressbar;
	};
}