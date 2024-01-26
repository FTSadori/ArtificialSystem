#pragma once
#include "Parser.h"
#include "Separator.h"
#include "Processor.h"

namespace Mova
{
	struct Version
	{
		uint8_t module_lvl = 1;
		uint8_t bits_lvl = 0;
		uint8_t types_lvl = 0;
		uint8_t jump_lvl = 0;
		uint8_t math_lvl = 0;
		uint8_t func_lvl = 0;

		Version() = default;

		Version(const std::string& line)
		{
			auto nums = Separator::split(line, '.');
			if (nums.size() != 6) throw ProcessorException("Wrong version number");
			module_lvl = Parser::from_string<int>(nums[0]);
			bits_lvl = Parser::from_string<int>(nums[1]);
			types_lvl = Parser::from_string<int>(nums[2]);
			jump_lvl = Parser::from_string<int>(nums[3]);
			math_lvl = Parser::from_string<int>(nums[4]);
			func_lvl = Parser::from_string<int>(nums[5]);
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << (int)module_lvl << "." << (int)bits_lvl << "." << (int)types_lvl << "." << (int)jump_lvl << "." << (int)math_lvl << "." << (int)func_lvl;
			return ss.str();
		}
	};
}