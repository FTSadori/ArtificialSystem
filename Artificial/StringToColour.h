#pragma once
#include "Colours.h"
#include <string>
#include <unordered_map>

namespace GUI 
{
	class StringToColour final
	{
	public:
		static std::unordered_map<std::string, Colours> strToColour;

		static Colours ToColour(const std::string& line)
		{
			if (strToColour.contains(line))
				return strToColour[line];
			return Colours::BLACK;
		}
	};
}