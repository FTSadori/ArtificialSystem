#pragma once
#include "Parser.h"
#include "VarVector.h"
#include "ConstantsVector.h"

namespace Mova
{
	class PointerOperations
	{
	public:
		static std::string ampersand(const std::string& word, VarVector& vv, size_t line, bool func = false)
		{
			if (word.size() < 2 || word[0] != '&')
				throw ProcessorException("Compile: Wrong '&' (Line " + Parser::to_string(line + 1) + ")");
			
			if (isdigit(word[1]))
				throw ProcessorException("Compile: '&' cannot be before numbers (Line " + Parser::to_string(line + 1) + ")");
		
			return ((func) ? "" : "v") + Parser::to_string(vv.get_ptr_by_name(word.substr(1), line));
		}

		static std::string asterisk(const std::string& word, VarVector& vv, ConstantsVector& cv, size_t line, bool func = false)
		{
			if (word.size() < 2 || word[0] != '*')
				throw ProcessorException("Compile: Wrong '*' (Line " + Parser::to_string(line + 1) + ")");
			
			if (isdigit(word[1]))
			{
				try
				{
					return Parser::to_string(cv.try_push_ptr(Parser::from_string<size_t>(word.substr(1))));
				}
				catch (...)
				{
					throw ProcessorException("Compile: Cannot convert number to uint (Line " + Parser::to_string(line + 1) + ")");
				}
			}
			return ((func) ? "" : "v") + Parser::to_string(vv.get_ptr_by_name(word.substr(1), line));
		}
	};
}