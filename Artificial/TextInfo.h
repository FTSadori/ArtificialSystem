#pragma once
#include "TextAttributes.h"
#include <string>

namespace GUI
{
	struct TextInfo
	{
		std::string text;
		TextAttributes attributes;
	};
}