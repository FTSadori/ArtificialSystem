#pragma once
#include "TextAttributes.h"
#include <string>

namespace GUI
{
	enum class TextType
	{
		MAIN,
		SECONDARY,
		THIRD,
		SELECTION,
		NONE,
	};

	struct TextInfo
	{
		std::string text;
		TextType type;
		TextAttributes attributes;
	};
}