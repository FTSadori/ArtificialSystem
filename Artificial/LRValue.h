#pragma once
#include "PointerOperations.h"

namespace Mova
{
	struct LValueData
	{
		std::string ptr = 0;
		bool asterisk = false;
		bool v = false;
	};

	struct RValueData
	{
		std::string first = 0;
		size_t second = 0;
		bool v = false;
	};

	class LRValue final
	{
	public:
		static LValueData get_lvalue(const std::string& word, VarVector& vv, ConstantsVector& cv, size_t line, bool func = false)
		{
			if (word.size() == 0)
				throw ProcessorException("Compile: Empty lvalue (Line " + Parser::to_string(line + 1) + ")");

			if (word[0] == '*')
				return { PointerOperations::asterisk(word, vv, cv, line), true };
			else if (isalpha(word[0]) || word[0] == '_')
				return { ((func) ? "" : "v") + Parser::to_string(vv.get_ptr_by_name(word, line)), false };
			else
				throw ProcessorException("Compile: Wrong lvalue (Line " + Parser::to_string(line + 1) + ")");
		}

		static RValueData get_rvalue(const std::string& word, VarVector& vv, ConstantsVector& cv, size_t line, bool func = false)
		{
			if (word.size() == 0)
				throw ProcessorException("Compile: Empty rvalue (Line " + Parser::to_string(line + 1) + ")");
			if (word.starts_with("*"))
				return { PointerOperations::asterisk(word, vv, cv, line), 2 };
			else if (word.starts_with("&"))
				return { PointerOperations::ampersand(word, vv, line), 0 };
			//else if (word.find("("))
			//	check_func_call(word, i);
			else if (isalpha(word[0]) || word[0] == '_')
				return { ((func) ? "" : "v") + Parser::to_string(vv.get_ptr_by_name(word, line)), 1 };
			else if (isdigit(word[0]) || word[0] == '+' || word[0] == '-')
			{
				try
				{
					return { Parser::to_string(cv.try_push_ptr(Parser::from_string<double>(word))), 1 };
				}
				catch (...)
				{
					throw ProcessorException("Compile: Cannot convert number to double (Line " + Parser::to_string(line + 1) + ")");
				}
			}
			else
				throw ProcessorException("Compile: Wrong rvalue (Line " + Parser::to_string(line + 1) + ")");
		}
	};
}