#pragma once
#include <unordered_map>
#include <string>
#include "DataQueue.h"
#include "Separator.h"
#include "CommandExceptions.h"

namespace Story
{
	class BaseEventHandler
	{
	public:
		std::string get(const std::string& talk_path);
		void set_new_pair(const std::string& talk_path, const std::string& script_path = "");
		void load(const std::string& data);
		std::string to_data();
		bool is_loaded();

	private:
		bool m_loaded = false;
		std::unordered_map<std::string, std::string> s_map;
	};
}