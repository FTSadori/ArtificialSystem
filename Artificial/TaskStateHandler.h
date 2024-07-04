#pragma once
#include "IncrementTask.h"
#include <unordered_map>
#include <string>

namespace Story
{
	enum class TaskState
	{
		CLOSED,
		OPEN,
		COMPLETE,
	};

	struct TaskInfo
	{
		TaskState state = TaskState::CLOSED;
		int totalAttempts = 0;
		int firstComplitionAttempts = -1;
		int minLinesOfCode = -1;
	};

	class TaskStateHandler
	{
	public:
		static std::unordered_map<std::string, TaskInfo> s_states;
	};
}