#include "TaskStateHandler.h"

namespace Story
{
	std::unordered_map<std::string, TaskInfo> TaskStateHandler::s_states = {
		{ "increment", {TaskState::CLOSED} },
		{ "sum", {TaskState::CLOSED} },
		{ "decrement", {TaskState::CLOSED} },
		{ "difference", {TaskState::CLOSED} },
		{ "intdiv", {TaskState::CLOSED} },
	};
}