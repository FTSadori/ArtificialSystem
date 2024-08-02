#include "TaskStateHandler.h"

namespace Story
{
	std::unordered_map<std::string, TaskInfo> TaskStateHandler::s_states = {
		{ "increment", {TaskState::CLOSED} },
		{ "sum", {TaskState::CLOSED} },
		{ "decrement", {TaskState::CLOSED} },
		{ "difference", {TaskState::CLOSED} },
		{ "intdiv", {TaskState::CLOSED} },
		{ "mult", {TaskState::CLOSED} },
		{ "cube", {TaskState::CLOSED} },
		{ "sumsquare", {TaskState::CLOSED} },
		{ "divpmod", {TaskState::CLOSED} },
		{ "hello", {TaskState::CLOSED} },
		{ "abc", {TaskState::CLOSED} },
		{ "case", {TaskState::CLOSED} },
		{ "brackets", {TaskState::CLOSED} },
		{ "root", {TaskState::CLOSED} },
		{ "rabbits", {TaskState::CLOSED} },
		{ "sort", {TaskState::CLOSED} },
		{ "shaking", {TaskState::CLOSED} },
		{ "bintodec", {TaskState::CLOSED} },
		{ "mzzz", {TaskState::CLOSED} },
		{ "mathtask", {TaskState::CLOSED} },
		{ "1337", {TaskState::CLOSED} },
	};
}