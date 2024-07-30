#pragma once
#include "BaseTask.h"

namespace Story
{
	class HelloWorldTask final : public BaseTask
	{
	public:
		HelloWorldTask() : BaseTask({ { } }, { {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'}}, "Hello, World!", "Print the 'Hello, World!'.", "CORE:\\.scripts\\chapter2\\d33_hello.txt") {}
	};

	class AbcTask final : public BaseTask
	{
	public:
		AbcTask() : BaseTask({ { } }, { {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'}}, "Abc", "Print the whole lowcase english alphabet 'abcdef...xyz'", "CORE:\\.scripts\\chapter2\\d35_abc.txt") {}
	};

	class ChangeCaseTask final : public BaseTask
	{
	public:
		ChangeCaseTask() : BaseTask({ {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\0'}}, {{'h', 'E', 'L', 'L', 'O', ',', ' ', 'w', 'O', 'R', 'L', 'D', '!'}}, "Change case", "Take a word (stop input when you get '\\0') and print it in reverse case, all characters except the English alphabet should not be changed. ('Mova!' => 'mOVA!')", "CORE:\\.scripts\\chapter2\\d37_case.txt") {}
	};

	class BracketsTask final : public BaseTask
	{
	public:
		BracketsTask() : BaseTask({ {'[', '{', '[', ']', '}', '(', ')', '\0'}, {'{', '}', '{', '\0'}, {'[', ']', '(', ')', '(', '\0'}}, {{']'}, {'}'}, {')'}}, "Brackets", "Take a string (stop input when you get '\0') and return what bracket (']', '}' or ')') needs to be added at the end of the line to make the line valid. ('{[]' => '}')", "CORE:\\.scripts\\chapter2\\d39_brackets.txt") {}
	};
}