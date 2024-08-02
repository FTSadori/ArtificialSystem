#pragma once
#include "BaseTask.h"

namespace Story
{
	class MultiplicationTask final : public BaseTask
	{
	public:
		MultiplicationTask() : BaseTask({ {1.0, 6.0}, {20.0, 3.0}, {4.0, 5.0} }, { {6.0}, {60.0}, {20.0} }, "Multiplication", "Return the product of two entered values.", "CORE:\\.scripts\\chapter2\\d22_mult.txt") {}
	};

	class CubeTask final : public BaseTask
	{
	public:
		CubeTask() : BaseTask({ {1.0}, {20.0}, {4.0} }, { {1.0}, {8000.0}, {64.0} }, "Cube", "Return the cube of the entered value.", "CORE:\\.scripts\\chapter2\\d24_cube.txt") {}
	};

	class SumOfSquaresTask final : public BaseTask
	{
	public:
		SumOfSquaresTask() : BaseTask({ {1.0, 6.0}, {20.0, 3.0}, {4.0, 5.0} }, { {37.0}, {409.0}, {41.0} }, "Sum of squares", "Return the value of (A*A + B*B), where A and B are entered values.", "CORE:\\.scripts\\chapter2\\d26_sumsquare.txt") {}
	};

	class DivPlusModTask final : public BaseTask
	{
	public:
		DivPlusModTask() : BaseTask({ {4.0, 2.0}, {20.0, 3.0}, {4.0, 5.0} }, { {2.0}, {8.0}, {4.0} }, "Integer division plus module", "Return the value of (A//B + A mod B), where A and B are entered values, // is integer division and 'mod' is module. B is never zero.", "CORE:\\.scripts\\chapter2\\d28_divpmod.txt") {}
	};
}