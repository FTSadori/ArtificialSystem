#pragma once
#include <unordered_set>
#include <string>

namespace Story
{
	class BannedNamesValidator
	{
	public:
		static bool is_banned(const std::string& name) {
			return s_banned_names.contains(name);
		}

	private:
		static std::unordered_set<std::string> s_banned_names;
	};
}