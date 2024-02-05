#pragma once
#include "MathExpression.h"

namespace Mova
{
	class ComparingExpression final
	{
	public:
		static std::vector<std::string> convert_to_raw_J(const std::vector<std::string>& parts, VarVector& vv, ConstantsVector& cv, size_t line)
		{
			if (parts.size() != 3 || !expressions.contains(parts[1]))
				throw ProcessorException("Compile: Wrong expression (Line " + Parser::to_string(line + 1) + ")");

			std::vector<std::string> res;
			for (auto& l : MathExpression::convert({ "_1", "=", parts[0] }, vv, cv, line))
				res.push_back(l);
			for (auto& l : MathExpression::convert({ "_2", "=", parts[2] }, vv, cv, line))
				res.push_back(l);
			std::string jump = "J v" + Parser::to_string(vv.get_ptr_by_name("_1", line)) + " v"
				+ Parser::to_string(vv.get_ptr_by_name("_2", line)) + " n " + expressions.at(parts[1]);
			res.push_back(jump);
			
			return res;
		}

	private:
		const static std::unordered_map<std::string, std::string> expressions;
	};
}