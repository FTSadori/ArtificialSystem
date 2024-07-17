#pragma once
#include <unordered_map>
#include <string>
#include "DataQueue.h"
#include "Separator.h"
#include "CommandExceptions.h"

namespace Story
{
	class DialogHandler
	{
	public:
		static std::string get(const std::string& talk_path);
		static void set_new_dialog(const std::string& talk_path, const std::string& script_path = "");
		static void load(const std::string& data);
		static std::string to_data();
		static bool is_loaded();

	private:
		static bool m_loaded;
		static std::unordered_map<std::string, std::string> s_dialog_map;
	};
}