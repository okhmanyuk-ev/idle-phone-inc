#pragma once

#include "screen.h"
#include "helpers.h"

namespace PhoneInc
{
	class LoadingScreen : public Scene::Actionable<Screen>
	{
	public:
		using Callback = std::function<void()>;
		using Tasks = std::vector<Callback>;

	public:
		LoadingScreen();

	public:
		void runTasks();

	public:
		void setTasks(const Tasks& value) { mTasks = value; }

	private:
		Tasks mTasks;
		
	private:
		std::shared_ptr<Helpers::StreetProgressbar> mProgressbar;
	};
}