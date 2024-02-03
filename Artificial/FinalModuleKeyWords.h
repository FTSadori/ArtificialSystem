#pragma once
#include <vector>
#include <string>

namespace Mova
{
	class FinalModuleKeyWords final
	{
	public:
		static const std::vector<std::string> c_keywords;

		static bool is_keyword(const std::string& word)
		{
			return (std::find(c_keywords.begin(), c_keywords.end(), word) != c_keywords.end());
		}
	};
}