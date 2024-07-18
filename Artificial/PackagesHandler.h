#pragma once
#include <unordered_map>
#include <string>

namespace Story
{
	class PackagesHandler
	{
	public:
		const static std::unordered_map<std::string, std::string> s_map;
	};
}