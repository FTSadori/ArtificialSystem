#pragma once
#include "BaseTask.h"

namespace Story
{
	class IncrementTask final : public BaseTask
	{
	public:
		IncrementTask() : BaseTask({ {1.0}, {2.0}, {532.0} }, { {2.0}, {3.0}, {533.0} }, "Increment", "---", "CORE:\\.scripts\\chapter2\\t_increment.txt") {}
	};

	class SumTask final : public BaseTask
	{
	public:
		SumTask() : BaseTask({ {1.0, 2.0}, {15.0, 2.0}, {5.0, 4.0} }, { {3.0}, {17.0}, {9.0} }, "Sum", "---", "CORE:\\.scripts\\chapter2\\t_sum.txt") {}
	};

	class DecrementTask final : public BaseTask
	{
	public:
		DecrementTask() : BaseTask({ {1.0}, {2.0}, {532.0} }, { {0.0}, {1.0}, {531.0} }, "Decrement", "---", "CORE:\\.scripts\\chapter2\\t_decrement.txt") {}
	};

	class DifferenceTask final : public BaseTask
	{
	public:
		DifferenceTask() : BaseTask({ {6.0, 2.0}, {15.0, 2.0}, {5.0, 4.0} }, { {4.0}, {13.0}, {1.0} }, "Difference", "---", "CORE:\\.scripts\\chapter2\\t_difference.txt") {}
	};

	class IntDivTask final : public BaseTask
	{
	public:
		IntDivTask() : BaseTask({ {6.0, 2.0}, {15.0, 2.0}, {5.0, 4.0} }, { {3.0}, {7.0}, {1.0} }, "Integer division", "---", "CORE:\\.scripts\\chapter2\\t_intdiv.txt") {}
	};
}