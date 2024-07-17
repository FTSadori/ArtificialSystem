#include "DialogHandler.h"

namespace Story
{
	std::unordered_map<std::string, std::string> DialogHandler::s_dialog_map;
	
	std::string DialogHandler::get(const std::string& talk_path)
	{
		if (!s_dialog_map.contains(talk_path))
			return "";
		else
			return s_dialog_map[talk_path];
	}

	void DialogHandler::set_new_dialog(const std::string& talk_path, const std::string& script_path)
	{
		if (script_path == "" && s_dialog_map.contains(talk_path))
			s_dialog_map.erase(talk_path);
		s_dialog_map[talk_path] = script_path;
	}

	void DialogHandler::load(const std::string& data)
	{
		s_dialog_map.clear();
		int i = 0;
		auto lines = Separator::split(data, ' ');
		try
		{
			while (i < lines.size())
			{
				s_dialog_map.insert(lines[i], lines[i + 1]);
				i += 2;
			}
		}
		catch (...) 
		{
			throw Commands::CommandException("(DialogHandler::load) Bad dialogs file");
		}
	}
	
	std::string DialogHandler::to_data()
	{
		std::string fulldata;
		for (const auto& [first, second] : s_dialog_map)
		{
			fulldata += first + "\n";
			fulldata += second + "\n";
		}

		return fulldata.substr(0, fulldata.size() - 1);
	}
}
