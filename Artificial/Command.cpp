#include "Command.h"
#include "Parser.h"
#include "Separator.h"
#include "CommandExceptions.h"

namespace Commands
{
	Command::Command(const std::string& line)
	{
		// Dividing by '>'
		size_t text_size = line.find('>');
		std::string text;
		if (text_size < line.size())
			text = line.substr(text_size + 1);

		std::string other = line.substr(0, text_size);

		// Dividing by '"' and ' '
		std::vector<std::string> divided;
		auto str = Separator::split(other, '"');
		for (size_t i = 0; i < str.size(); ++i)
		{
			if (i % 2 == 0)
			{
				auto by_spaces = Separator::split(str[i], ' ');
				for (const auto& s : by_spaces)
					if (s.size() != 0) 
						divided.push_back(s);
			}
			else
			{
				divided.push_back('"' + str[i]);
			}
		}

		if (text != "")
			divided.push_back(text);

		// Load to map
		if (divided.size() == 0)
			return;

		for (size_t i = 0; i < divided.size(); ++i)
			divided[i] = divided[i].starts_with('"') ? divided[i].substr(1) : divided[i];

		m_parameters["name"] = divided[0];
		size_t parameter = 1;
		for (size_t i = 1; i < divided.size(); ++i)
		{
			if (divided[i].size() > 0 && divided[i][0] == ':')
			{
				parameter = 0;
				m_parameters[divided[i]] = divided[i + 1];
				i++;
			}
			else if (parameter > 0)
			{
				m_parameters[Parser::to_string(parameter++)] = divided[i];
			}
		}
	}

	std::string Command::get(const std::string& _key) const
	{
		return m_parameters.at(_key);
	}

	bool Command::has(const std::string& _key) const
	{
		return m_parameters.find(_key) != m_parameters.end();
	}
}