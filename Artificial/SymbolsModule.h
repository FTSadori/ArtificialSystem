#pragma once
#include "Version.h"
#include <unordered_map>
#include "ProcessorExceptions.h"
#include "Separator.h"

namespace Mova
{
	class SymbolsModule final
	{
	private:
		static const std::unordered_map<std::string, std::string> map;

	public:
		static std::vector<std::string> compile(const std::vector<std::string> code, uint8_t bits)
		{
			std::vector<std::string> res;
			for (size_t i = 0; i < code.size(); ++i)
			{
				auto args = Separator::split(code[i], ' ');
				if (args.size() == 0)
					throw ProcessorException("Compiler: Line " + Parser::to_string(i + 1) + ": Empty line");
				
				std::string str;
				if (map.contains(args[0]))
					str += map.at(args[0]);
				else
					throw ProcessorException("Compiler: Line " + Parser::to_string(i + 1) + ": Wrong command");
				
				try
				{
					for (size_t j = 1; j < args.size(); ++j)
					{
						int num = Parser::from_string<int>(args[j]);
						if (num < 0)
						{
							num += pow(2, bits);
						}
						std::string num_bits = Parser::to_bits(num);
						if (num_bits.size() < bits)
							num_bits = std::string(bits - num_bits.size(), '0') + num_bits;
						str += num_bits;
					}
				}
				catch (...)
				{
					throw ProcessorException("Compiler: Line " + Parser::to_string(i + 1) + ": Cannot convert to integer");
				}

				res.push_back(str);
			}
			return res;
		}
	};
}