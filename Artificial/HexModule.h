#pragma once
#include "Version.h"
#include <unordered_map>
#include "ProcessorExceptions.h"

namespace Mova
{
	class HexModule final
	{
	private:
		static const std::unordered_map<char, std::string> map;

	public:
		static std::vector<std::string> compile(const std::vector<std::string> code)
		{
			std::vector<std::string> res;
			for (size_t i = 0; i < code.size(); ++i)
			{
				std::string str;
				for (char c : code[i])
				{
					char lc = tolower(c);
					if (map.contains(lc))
						str += map.at(lc);
					else
						throw ProcessorException("Compiler: Line " + Parser::to_string(i + 1) + ": Wrong symbol '" + std::string(1, lc) + "'");
				}
				res.push_back(str);
			}
			return res;
		}
	};
}