#include "ItemUsingHandler.h"

namespace Story
{
	std::unordered_map<std::string, std::string> ItemUsingHandler::s_event_to_script = {
		{ "[Water flower] OVERWORLD:\\largetree\\darkgorge3\\door\\blockeddoor\\anotherdoor", "CORE:\\.scripts\\chapter1\\u_fl.txt" },
		{ "[Glass key] OVERWORLD:\\largetree\\darkgorge3", "CORE:\\.scripts\\chapter1\\u_key_1.txt" },
		{ "[Glass key] OVERWORLD:\\largetree\\darkgorge3\\door\\blockeddoor", "CORE:\\.scripts\\chapter1\\u_key_2.txt" },
		{ "[Weeping flower] OVERWORLD:\\largetree\\darkgorge3\\door", "CORE:\\.scripts\\chapter1\\u_wipfl_1.txt" },
		{ "[Weeping flower] OVERWORLD:\\largetree\\darkgorge3\\door\\blockeddoor\\anotherdoor", "CORE:\\.scripts\\chapter1\\u_wipfl_2.txt" },
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