#include "MovaVersionHandler.h"

namespace Mova
{
	std::array<int, 6> MovaVersionHandler::s_version = { 0, 0, 0, 0, 0, 0 };

	void MovaVersionHandler::add_package(const std::string& name)
	{
		if (name.starts_with("mova"))
		{
			auto parts = Separator::split(name.substr(4), '.');
			try
			{
				for (int i = 0; i < s_version.size(); ++i)
					s_version[i] = std::max(s_version[i], Parser::from_string<int>(parts[i]));
			}
			catch (...) { return; }
		}
	}

	std::string MovaVersionHandler::get_line()
	{
		std::string line = "v";
		for (int n : s_version)
			line += Parser::to_string(n) + ".";
		return line.substr(0, line.size() - 1);
	}

	void MovaVersionHandler::reset()
	{
		s_version = { 0, 0, 0, 0, 0, 0 };
	}
}