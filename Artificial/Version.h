#pragma once
#include "Parser.h"
#include "Separator.h"

namespace Mova
{
	struct Version
	{
		uint8_t bits_lvl = 0;
		uint8_t types_lvl = 0;
		uint8_t jump_lvl = 0;
		uint8_t var_lvl = 0;
		uint8_t func_lvl = 0;

		Version(const std::string& line)
		{
			auto nums = Separator::split(line, '.');
			bits_lvl = Parser::from_string<uint8_t>(nums[0]);
			types_lvl = Parser::from_string<uint8_t>(nums[1]);
			jump_lvl = Parser::from_string<uint8_t>(nums[2]);
			var_lvl = Parser::from_string<uint8_t>(nums[3]);
			func_lvl = Parser::from_string<uint8_t>(nums[4]);
		}

		std::string to_string()
		{
			std::stringstream ss;
			ss << (int)bits_lvl << "." << (int)types_lvl << "." << (int)jump_lvl << "." << (int)var_lvl << "." << (int)func_lvl;
			return ss.str();
		}
	};
}