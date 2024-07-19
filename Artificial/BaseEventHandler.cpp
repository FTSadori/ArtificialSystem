#include "BaseEventHandler.h"

namespace Story
{
	std::unordered_map<std::string, std::string> BaseEventHandler::s_map;
	bool BaseEventHandler::m_loaded = false;

	std::string BaseEventHandler::get(const std::string& talk_path)
	{
		if (!s_map.contains(talk_path))
			return "";
		else
			return s_map[talk_path];
	}

	void BaseEventHandler::set_new_dialog(const std::string& talk_path, const std::string& script_path)
	{
		if (script_path == "" && s_map.contains(talk_path))
			s_map.erase(talk_path);
		else
			s_map[talk_path] = script_path;
	}

	void BaseEventHandler::load(const std::string& data)
	{
		s_map.clear();
		int i = 0;
		auto lines = Separator::split(data, '\n');
		try
		{
			while (i < lines.size())
			{
				s_map.emplace(lines[i], lines[i + 1]);
				i += 2;
			}
		}
		catch (...)
		{
			throw Commands::CommandException("(BaseEventHandler::load) Bad event file");
		}
		m_loaded = true;
	}

	std::string BaseEventHandler::to_data()
	{
		std::string fulldata;
		for (const auto& [first, second] : s_map)
		{
			fulldata += first + "\n";
			fulldata += second + "\n";
		}

		return fulldata.substr(0, fulldata.size() - 1);
	}

	bool BaseEventHandler::is_loaded()
	{
		return m_loaded;
	}
}
