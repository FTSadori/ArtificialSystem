#pragma once
#include "LRValue.h"

namespace Mova
{
	class MathExpression final
	{
	private:
		static const std::unordered_map<std::string, std::string> math_expr;

	public:
		static std::vector<std::string> convert(const std::vector<std::string>& parts, VarVector& vv, ConstantsVector& cv, size_t line)
		{
			std::vector<std::string> res;

			if (parts.size() != 3 || !math_expr.contains(parts[1]))
				throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(line + 1) + ")");
			if (parts[1] == "=" && isalpha(parts[0][0]))
				vv.push_var(parts[0], line);
			
			auto left = LRValue::get_lvalue(parts[0], vv, cv, line);
			auto right = LRValue::get_rvalue(parts[2], vv, cv, line);
			
			std::string operation = math_expr.at(parts[1]) + " " 
				+ left.ptr + " " 
				+ right.first + " "
				+ Parser::to_string(right.second);
			if (left.asterisk) operation += " 0";

			if (parts[1] == "=") 
				if (parts[0][0] == '*')
					res.push_back("Z " + Parser::to_string(left.ptr) + " 0");
				else
					res.push_back("Z " + Parser::to_string(left.ptr));
			res.push_back(operation);

			return res;
		}
	};
}
