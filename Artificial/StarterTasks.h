#pragma once
#include "BaseTask.h"

namespace Story
{
	class IncrementTask final : public BaseTask
	{
	public:
		IncrementTask() : BaseTask({ {1.0}, {2.0}, {532.0} }, { {2.0}, {3.0}, {533.0} }, "Increment", "Return a value that is 1 greater than the entered value.", "CORE:\\.scripts\\chapter2\\d8_increment.txt") {}
	};

	class SumTask final : public BaseTask
	{
	public:
		SumTask() : BaseTask({ {1.0, 2.0}, {15.0, 2.0}, {5.0, 4.0} }, { {3.0}, {17.0}, {9.0} }, "Sum", "Return the sum of two entered values.", "CORE:\\.scripts\\chapter2\\d12_sum.txt") {}
	};

	class DecrementTask final : public BaseTask
	{
	public:
		DecrementTask() : BaseTask({ {1.0}, {2.0}, {532.0} }, { {0.0}, {1.0}, {531.0} }, "Decrement", "Return a value that is 1 less than the entered value. (Entered value starts from 1)", "CORE:\\.scripts\\chapter2\\d14_decrement.txt") {}
	};

	class DifferenceTask final : public BaseTask
	{
	public:
		DifferenceTask() : BaseTask({ {6.0, 2.0}, {15.0, 2.0}, {5.0, 4.0} }, { {4.0}, {13.0}, {1.0} }, "Difference", "Return the difference between two entered values. (The first one is always bigger)", "CORE:\\.scripts\\chapter2\\d16_difference.txt") {}
	};

	class IntDivTask final : public BaseTask
	{
	public:
		IntDivTask() : BaseTask({ {6.0}, {15.0}, {5.0} }, { {3.0}, {7.0}, {2.0} }, "Integer division by 2", "Return the integer fraction of dividing the entered number by 2.", "CORE:\\.scripts\\chapter2\\d18_intdiv.txt") {}
	};
}