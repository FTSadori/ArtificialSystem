#pragma once
#include "Colours.h"
#include <string>
#include <map>

namespace GUI 
{
	class StringToColour final
	{
	public:
		static std::map<std::string, Colours> strToColour;

		static Colours ToColour(const std::string& line)
		{
			if (strToColour.contains(line))
				return strToColour[line];
			return Colours::BLACK;
		}
	};
}