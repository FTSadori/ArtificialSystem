#pragma once
#include "BaseTask.h"

namespace Story
{
	class RootsTask final : public BaseTask
	{
	public:
		RootsTask() : BaseTask({ {6.0}, {144.0}, {399.0} }, { {2.0}, {12.0}, {19.0} }, "Square root", "Return the square root from the entered value.", "CORE:\\.scripts\\chapter2\\d44_roots.txt") {}
	};

	class RabbitsTask final : public BaseTask
	{
	public:
		RabbitsTask() : BaseTask({ {0.0}, {6.0}, {11.0}, {17.0} }, { {0.0}, {8.0}, {89.0}, {1597.0} }, "Rabbits", "Let there be 0 rabbits on day zero. The first baby rabbit is born on day one. Then every day all the baby rabbits grow up and all the adult rabbits give birth to new baby rabbits. How many rabbits will there be on day n?", "CORE:\\.scripts\\chapter2\\d46_rabbits.txt") {}
	};

	class SortTask final : public BaseTask
	{
	public:
		SortTask() : BaseTask({ {0.0, 5.0, 1.0, 9.0, 5.0, 8.0, 1.0, 3.0, 4.0, 2.0, -1.0} }, { {0.0, 1.0, 1.0, 2.0, 3.0, 4.0, 5.0, 5.0, 8.0, 9.0} }, "Sort", "Sort the entered array. Stop the input when you get -1. Do not include -1 to the array.", "CORE:\\.scripts\\chapter2\\d48_sort.txt") {}
	};
}