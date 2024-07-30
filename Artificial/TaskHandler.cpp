#include "TaskHandler.h"

namespace Story
{
	std::unordered_map<std::string, std::shared_ptr<BaseTask>> TaskHandler::s_tasks = {
		{ "increment", std::make_unique<IncrementTask>() },
		{ "sum", std::make_unique<SumTask>() },
		{ "decrement", std::make_unique<DecrementTask>() },
		{ "difference", std::make_unique<DifferenceTask>() },
		{ "intdiv", std::make_unique<IntDivTask>() },
		{ "mult", std::make_unique<MultiplicationTask>() },
		{ "cube", std::make_unique<CubeTask>() },
		{ "sumsquare", std::make_unique<SumOfSquaresTask>() },
		{ "divpmod", std::make_unique<DivPlusModTask>() },
		{ "hello", std::make_unique<HelloWorldTask>() },
		{ "abc", std::make_unique<AbcTask>() },
		{ "case", std::make_unique<ChangeCaseTask>() },
		{ "brackets", std::make_unique<BracketsTask>() },
		{ "root", std::make_unique<RootsTask>() },
		{ "rabbits", std::make_unique<RabbitsTask>() },
		{ "sort", std::make_unique<SortTask>() },
		{ "shaking", std::make_unique<ShakingTask>() },
		{ "bintodec", std::make_unique<BinaryToDecimalTask>() },
		{ "mzzz", std::make_unique<MzzzTask>() },
		{ "mathtask", std::make_unique<MathTask>() },
		{ "1337", std::make_unique<LeetTask>() },
	};
}