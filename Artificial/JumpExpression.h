#pragma once
#include "ComparingExpression.h"
#include "MarkVector.h"

namespace Mova
{
	class JumpExpression final
	{
	public:
		static std::vector<std::string> convert_goto(const std::vector<std::string>& parts, VarVector& vv, ConstantsVector& cv, MarkVector& mv, size_t line)
		{
			std::vector<std::string> res;

			if (parts.size() == 2 || parts.size() == 5)
			{
				if (!vv.check_name(parts[1], line))
					throw ProcessorException("Compile: Wrong mark name (Line " + Parser::to_string(line + 1) + ")");

				mv.meet_goto(parts[1], line);
				res.push_back("J 0 0 m" + parts[1]);

				if (parts.size() == 5)
				{
					res = ComparingExpression::convert_to_raw_J({ parts[2], parts[3], parts[4] }, vv, cv, line);
					auto jparts = Separator::split(res.back(), ' ');
					jparts[3] = "m" + parts[1];
					res.pop_back();
					res.push_back(Separator::join(jparts, ' '));
				}

				return res;
			}
			else
				throw ProcessorException("Compile: Wrong expression (Line " + Parser::to_string(line + 1) + ")");
		}
	};
}