#pragma once
#include <unordered_map>
#include <vector>
#include <string>
#include "Parser.h"

namespace Mova
{
	class ConstantsVector final
	{
	public:
		size_t try_push_ptr(double num)
		{
			if (!m_constants.contains(num))
				m_constants.emplace(num, m_constants.size());
			return m_constants[num];
		}

		size_t get_size()
		{
			return m_constants.size();
		}

		std::vector<std::string> convert_to_code()
		{
			std::vector<std::string> res;

			for (const auto& [num, ptr] : m_constants)
			{
				std::string first = (num >= 0) ? "SUM" : "SUB";
				first += " " + Parser::to_string(ptr);

				int k = 0;
				double cp = abs(num);
				while (cp - trunc(cp) >= eps && k <= 8)
				{
					++k;
					cp *= 10;
				}
				first += " " + Parser::to_string(int(cp));
				res.push_back(first);

				if (k != 0)
				{
					std::string second = "SHR " + Parser::to_string(ptr) + " " + Parser::to_string(k);
					res.push_back(second);
				}
			}

			return res;
		}

	private:
		std::unordered_map<double, size_t> m_constants;

		const double eps = 1e-6;
	};
}