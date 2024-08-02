#pragma once
#include "BaseTask.h"

namespace Story
{
	class ShakingTask final : public BaseTask
	{
	public:
		ShakingTask() : BaseTask({ {'H', ']', 'e', 'l', '}', 'l', '[', 'o', '\\', ',', ' ', 'W', '/', 'o', '/', 'r', '|', 'l', 'd', '\\', '!', '/', '\\', '\0'}}, {{'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'}}, "Shaking", "Get the string from the input stream (until you get the '\\0' symbol). Return the string without '\\|/[]{}' symbols.", "CORE:\\.scripts\\chapter2\\d52_shaking.txt") {}
	};

	class BinaryToDecimalTask final : public BaseTask
	{
	public:
		BinaryToDecimalTask() : BaseTask({ {'1', '0', '0', '\0'}, {'1', '0', '1', '0', '\0'}, {'1', '1', '1', '0', '\0'}}, {{4.0}, {10.0}, {14.0}}, "Binary to decimal", "Get the string (until you get the '\\0' symbol) that represents the binary number. Return its decimal equivalent.", "CORE:\\.scripts\\chapter2\\d59_bintodec.txt") {}
	};

	class MzzzTask final : public BaseTask
	{
	public:
		MzzzTask() : BaseTask({ {3.0}, {8.0}, {22.0}, {31.0}, {9.0}, {15.0}, {26.0}, {29.0} }, { {3.0}, {2,0}, {3.0}, {4.0}, {3.0}, {3.0}, {3.0}, {5.0} }, "MZzzzz...", "Get a number and return the minimum number of letters that can be used to split the number according to the following rule: 'M' - 10, 'Z' - 6, 'z' - 4, '.' - 1.", "CORE:\\.scripts\\chapter2\\d64_mzzz.txt") {}
	};

	class MathTask final : public BaseTask
	{
	public:
		MathTask() : BaseTask({ {'1', '+', '0', '\0'}, {'1', '3', '+', '5', '\0'}, {'3', '6', '+', '1', '5', '\0'}}, {{1.0}, {18.0}, {51.0}}, "Math equations", "Get a string (until you get the '\\0' symbol) that represents the mathematical equation. It consists of a decimal number followed by a plus sign followed by another decimal number. Return the answer to this equation (as a number).", "CORE:\\.scripts\\chapter2\\d70_mathtask.txt") {}
	};

	class LeetTask final : public BaseTask
	{
	public:
		LeetTask() : BaseTask({ {'H', '3', '1', '1', '0', ',', ' ', 'W', '0', 'r', '1', '6', '!', '7', 'i', '7', '5', '\0'}}, {{'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', 't', 'i', 't', 's'}}, "1337", "Get a line (until you get the '\\0' symbol) and return the line with following symbols changed: '3' to 'e', '1' to 'l', '7' to 't', '5' to 's', '6' to 'd', '0' to 'o'.", "CORE:\\.scripts\\chapter2\\d80_1337.txt") {}
	};
}