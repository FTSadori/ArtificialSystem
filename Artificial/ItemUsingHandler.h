#pragma once
#include <unordered_map>
#include "FullPath.h"
#include <string>

namespace Story
{
	class ItemUsingHandler
	{
	public:
		static std::string get_script_for_event(const Memory::FullPath& position, const std::string& item_name);

	private:
		static std::unordered_map<std::string, std::string> s_event_to_script;
	};
}