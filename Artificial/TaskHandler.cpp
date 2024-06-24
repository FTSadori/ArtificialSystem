#include "TaskHandler.h"

namespace Story
{
	std::unordered_map<std::string, std::shared_ptr<BaseTask>> TaskHandler::s_tasks = {
		{ "increment", std::make_unique<IncrementTask>() },
	};
}