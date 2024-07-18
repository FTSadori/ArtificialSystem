#pragma once
#include <unordered_map>
#include <string>

namespace Story
{
	class SurroundingsHandler
	{
	public:
		static const std::unordered_map<std::string, std::string> s_map;
	};
}