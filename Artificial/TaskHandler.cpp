#include "TaskHandler.h"

namespace Story
{
	std::unordered_map<std::string, std::shared_ptr<BaseTask>> TaskHandler::s_tasks = {
		{ "increment", std::make_unique<IncrementTask>() },
		{ "sum", std::make_unique<SumTask>() },
		{ "decrement", std::make_unique<DecrementTask>() },
		{ "difference", std::make_unique<DifferenceTask>() },
		{ "intdiv", std::make_unique<IntDivTask>() },
	};
}