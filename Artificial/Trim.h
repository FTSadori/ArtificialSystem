#pragma once
#include <string>

class Trim final
{
public:
	static std::string left_trim(const std::string& str, const std::string& sym = " ")
	{
		if (str.size() == 0) return str;
		std::string res = str;
		while (sym.find(res[0]) < sym.size())
			res = res.substr(1);
		return res;
	}
};