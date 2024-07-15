#pragma once
#include <unordered_map>
#include <string>

namespace Story
{
	class BannedFileExtensionsHandler
	{
	public:
		static uint8_t get_min_level(const std::string& name) {
			if (name.size() < 5) return 0;

			auto extention = name.substr(name.size() - 5);
			if (s_banned.contains(extention))
				return s_banned[extention];
			return 0;
		}

	private:
		static std::unordered_map<std::string, uint8_t> s_banned;
	};
}