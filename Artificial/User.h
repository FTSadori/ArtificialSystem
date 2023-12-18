#pragma once
#include <string>

namespace Commands 
{
	class User
	{
	public:
		User(const std::string& _name, bool _sudo, uint8_t _lvl)
			: c_name(_name), c_sudo(_sudo), c_lvl(_lvl) 
		{}

		const bool c_sudo;
		const uint8_t c_lvl;
		const std::string c_name;
	};
}