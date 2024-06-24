#include "StringToColour.h"

namespace GUI
{
	std::unordered_map<std::string, Colours> StringToColour::strToColour = {
		{ "black", Colours::BLACK }, { "blue", Colours::BLUE }, { "gray", Colours::GRAY }, { "green", Colours::GREEN },
		{ "lightblue", Colours::LIGHT_BLUE }, { "lightgray", Colours::LIGHT_GRAY }, { "lightgreen", Colours::LIGHT_GREEN }, { "lightred", Colours::LIGHT_RED },
		{ "lightturquoise", Colours::LIGHT_TURQUOISE }, { "lightviolet", Colours::LIGHT_VIOLET }, { "lightyellow", Colours::LIGHT_YELLOW }, { "red", Colours::RED },
		{ "turquoise", Colours::TURQUOISE }, { "violet", Colours::VIOLET }, { "white", Colours::WHITE }, { "yellow", Colours::YELLOW },
	};
}