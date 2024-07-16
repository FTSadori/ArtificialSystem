#include "ItemUsingHandler.h"

namespace Story
{
	std::unordered_map<std::string, std::string> ItemUsingHandler::s_event_to_script = {
		{ "[shit] BAZA:\\ass", "BAZA:\\happy.txt" }
	};

	std::string ItemUsingHandler::get_script_for_event(const Memory::FullPath& position, const std::string& item_name)
	{
		std::string full = "[" + item_name + "] " + position.full_path_str();
		if (s_event_to_script.contains(full))
			return s_event_to_script[full];
		else
			return "";
	}
}