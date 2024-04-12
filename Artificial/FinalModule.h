#pragma once
#include "Version.h"
#include <unordered_map>
#include "ProcessorExceptions.h"
#include "Separator.h"
#include <stack>
#include "Trim.h"
#include "JumpExpression.h"

namespace Mova
{
	class FinalModule final
	{
	private:
		static std::vector<std::string> check_undefined(const std::vector<std::string>& parts, VarVector& vv, ConstantsVector& cv, MarkVector& mv, size_t code_len, size_t line)
		{
			if (parts.size() == 3)
				return MathExpression::convert(parts, vv, cv, line);
			else if (parts.size() == 1)
			{
				if (parts[0].find(":") < parts[0].size())
				{
					mv.add_mark(parts[0].substr(0, parts[0].size() - 1), code_len, line);
					return {};
				}
			}
			else
				throw ProcessorException("Compile: Wrong expression (Line " + Parser::to_string(line + 1) + ")");
		}

		static std::vector<std::string> check_input(const std::vector<std::string>& parts, VarVector& vv, ConstantsVector& cv, MarkVector& mv, size_t code_len, size_t line)
		{
			if (parts.size() != 2)
				throw ProcessorException("Compile: Wrong expression (Line " + Parser::to_string(line + 1) + ")");

			vv.push_var(parts[1], line);
			auto data = LRValue::get_lvalue(parts[1], vv, cv, line);
			return { "IN " + data.ptr + ((data.asterisk) ? " 0" : "") };
		}

		static std::vector<std::string> check_all(const std::vector<std::string>& parts, VarVector& vv, ConstantsVector& cv, MarkVector& mv, size_t code_len, size_t line)
		{
			if (parts[0] == "goto")
				return JumpExpression::convert_goto(parts, vv, cv, mv, line);
			else if (parts[0] == ">>")
				return check_input(parts, vv, cv, mv, code_len, line);
			else
				return check_undefined(parts, vv, cv, mv, code_len, line);
		}

	public:
		static std::vector<std::string> compile(const std::vector<std::string> code, const Version& version)
		{
			VarVector vv;
			ConstantsVector cv;
			MarkVector mv;
			
			std::vector<std::string> part1;
			for (size_t i = 0; i < code.size(); ++i)
			{
				auto parts = Separator::split(Trim::left_trim(code[i]), ' ');
				if (parts.size() == 0) continue;

				auto lines = check_all(parts, vv, cv, mv, part1.size(), i);
				for (auto& p : lines) part1.push_back(p);
			}

			std::vector<std::string> res = cv.convert_to_code();
			size_t c_size = res.size();
			for (auto& p : mv.convert(vv.convert(part1, cv.get_size()), c_size))
				res.push_back(p);

			return res;
		}
	};
}