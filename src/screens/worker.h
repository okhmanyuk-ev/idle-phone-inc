#pragma once

#include <shared/all.h>
#include "factory.h"

namespace PhoneInc
{
	enum class WorkerAnimation
	{
		Idle,
		Working
	};
	
	class Factory::Worker : public Scene::MappedAniSprite<WorkerAnimation>
	{
	public:
		Worker();

	public:
		void refresh();

	public:
		auto getLevel() const { return mLevel; }
		void setLevel(int value);

	private:
		int mLevel = 0;

	private:
		const static inline StateMap Animations = {
			{ Animation::Idle, "idle" },
			{ Animation::Working, "working" }
		};
	};
}