#pragma once
#include <string>
#include <vector>

class Separator final
{
public:
	static std::vector<std::string> split(const std::string& line, char sep)
	{
		std::vector<std::string> lines;
		size_t it = 0;
		for (size_t i = 0; it < line.size(); i = it + 1)
		{
			it = line.find(sep, i);
			lines.push_back(line.substr(i, (it - i)));
		}
		return lines;
	}

	static std::string join(const std::vector<std::string>& lines, char sep)
	{
		if (lines.empty()) return "";

		std::string res = "";
		for (const auto& line : lines)
			res += sep + line;
		res = res.substr(1);
		return res;
	}

	static std::vector<std::string> split_by(const std::string& line, int num)
	{
		std::vector<std::string> lines;
		std::string copy = line;
		for (size_t i = 0; i < line.size(); i += num)
		{
			lines.push_back(copy.substr(0, num));
			if (copy.size() > num)
				copy = copy.substr(num);
		}
		return lines;
	}
};
