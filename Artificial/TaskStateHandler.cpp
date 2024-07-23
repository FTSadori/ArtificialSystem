#include "TaskStateHandler.h"

namespace Story
{
	std::unordered_map<std::string, TaskInfo> TaskStateHandler::s_states = {
		{ "increment", {TaskState::CLOSED} },
	};
}