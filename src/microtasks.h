#pragma once

#include <sky/sky.h>

#define MICROTASKS sky::Locator<PhoneInc::Microtasks>::Get()

namespace PhoneInc
{
	class Microtasks
	{
	public:
		struct TaskReadyEvent { };
		struct TaskCompletedEvent { };

		struct Task
		{
			enum class Type
			{
				None,
				UnlockRooms,
				WarehouseLevel,
				ProductLevel,
				ManagerLevel,
				WorkerLevel,
			};

			Type type;
			int target;
			int reward;
			std::string locale;
		};

	public:
		static std::string TaskTypeToString(Task::Type type);
		static Task::Type StringToTaskType(const std::string& name);

	public:
		Microtasks();

	public:
		void checkForCompletion();
		void complete();
		void clear();

	public:
		const Task& getCurrentTask() const;
		const auto& getTasks() const { return mTasks; }
		bool hasUnfinishedTasks() const;
		bool isReady() const { return mReady; }

	private:
		std::vector<Task> mTasks;
		bool mReady = false;
	};
}
