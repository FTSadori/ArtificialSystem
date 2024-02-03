#pragma once
#include "Version.h"
#include <unordered_map>
#include "ProcessorExceptions.h"
#include "Separator.h"
#include <stack>
#include "Trim.h"
#include "MathExpression.h"

namespace Mova
{
	struct FuncInfo
	{
		size_t args = 0;
		size_t ptr = 0;
	};

	class FinalModule final
	{
	private:
		static std::unordered_map<double, size_t> s_constants;
		static std::unordered_map<std::string, size_t> s_marks;
		static std::unordered_map<std::string, FuncInfo> s_functions;
		static size_t s_current_nesting_lvl;
		static size_t s_current_var_ptr;

	public:

		static const std::vector<std::string> comparison_expr;

	private:
		/*
		static void precompile(const std::vector<std::string>& code)
		{
			s_constants.clear();
			//s_vars.clear();
			s_marks.clear();
			s_functions.clear();

			std::stack<char> stack;

			for (size_t i = 0; i < code.size(); ++i)
			{		 
				auto parts = Separator::split(Trim::left_trim(code[i]), ' ');
				if (parts.size() == 0) continue;			 
				if (parts[0] == "if" || parts[0] == "while")
				{
					check_ifwhile(parts, i);
					stack.push('s');
				}
				else if (parts[0] == "for")
				{
					check_for(parts, i);
					stack.push('s');
				}
				else if (parts[0] == "goto")
					check_goto(parts, i);
				else if (parts[0] == "func")
				{
					check_func(parts, i);
					if (stack.empty())
						stack.push('f');
					else
						throw ProcessorException("Precompile: Function cannot be inside while/for/if/func block (Line " + Parser::to_string(i + 1) + ")");
				}
				else if (parts[0] == "return")
				{
					check_return(parts, i);
					if (!stack.empty() && stack.top() == 'f')
						stack.pop();
					else
						throw ProcessorException("Precompile: Wrong return statement (Line " + Parser::to_string(i + 1) + ")");
				}
				else if (parts[0] == "end")
				{
					if (!stack.empty() && stack.top() == 's')
						stack.pop();
					else
						throw ProcessorException("Precompile: Wrong end statement (Line " + Parser::to_string(i + 1) + ")");
				}
				else check_undefined(parts, i);
			}
			if (!stack.empty())
			{
				if (stack.top() == 's')
					throw ProcessorException("Precompile: Need end keyword");
				if (stack.top() == 'f')
					throw ProcessorException("Precompile: Need return keyword");
			}
		}

		static void check_condition(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() != 3 ||
				std::find(comparison_expr.begin(), comparison_expr.end(), parts[1]) == comparison_expr.end())
				throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
			check_rvalue(parts[0], i);
			check_rvalue(parts[2], i);
		}

		static void check_mark(const std::string& word, size_t i)
		{
			if (word.size() == 0)
				throw ProcessorException("Precompile: Empty rvalue (Line " + Parser::to_string(i + 1) + ")");
			if (word.back() != ':')
				throw ProcessorException("Precompile: Mark must end with ':' symbol (Line " + Parser::to_string(i + 1) + ")");
			auto mark = word.substr(0, word.size() - 1);
			if (s_marks.contains(mark))
				throw ProcessorException("Precompile: This mark is already exists (Line " + Parser::to_string(i + 1) + ")");
			s_marks.emplace(mark, 0);
		}

		static void check_goto(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() == 2 && s_marks.contains(parts[1]))
				return;
			if (parts.size() == 5 && s_marks.contains(parts[1]))
			{
				std::vector<std::string> p = { parts[2], parts[3], parts[4] };
				check_condition(p, i);
				return;
			}

			throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_return(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() == 1)
				return;
			if (parts.size() == 2)
			{
				check_rvalue(parts[1], i);
				return;
			}
		
			throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_ifwhile(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() == 4)
			{
				std::vector<std::string> p = { parts[1], parts[2], parts[3] };
				check_condition(p, i);
				return;
			}

			throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_for(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() == 7 && parts[2] == "in" && parts[5] == "by")
			{
				check_rvalue(parts[3], i);
				check_rvalue(parts[4], i);
				check_rvalue(parts[6], i);
				check_var(parts[1], i, false);
				//if (!s_vars.contains(parts[1]))
				//	s_vars.emplace(parts[1], s_vars.size());
			}
			else
				throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_end(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() != 1)
				throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_func(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() == 2)
			{
				auto second_sep = Separator::split(parts[1], '(');
				if (second_sep.size() == 2)
				{
					if (s_functions.contains(second_sep[0]))
						throw ProcessorException("Precompile: Function is already defined (Line " + Parser::to_string(i + 1) + ")");
					if (second_sep[1].ends_with(')'))
					{
						auto args_sep = Separator::split(second_sep[1].substr(0, second_sep[1].size() - 1), ',');
						for (const auto& arg : args_sep)
							check_var(arg, i, false);

						s_functions.emplace(second_sep[0], FuncInfo{ args_sep.size(), 0 });
						return;
					}
				}
			}

			throw ProcessorException("Precompile: Wrong expression (do not use any spaces besides after 'func' keyword) (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_func_call(const std::string& word, size_t i)
		{
			auto second_sep = Separator::split(word, '(');
			if (second_sep.size() == 2)
			{
				if (!s_functions.contains(second_sep[0]))
					throw ProcessorException("Precompile: Function is undefined (Line " + Parser::to_string(i + 1) + ")");
				if (second_sep[1].ends_with(')'))
				{
					auto args_sep = Separator::split(second_sep[1].substr(0, second_sep[1].size()), ',');
					for (const auto& arg : args_sep)
						check_rvalue(arg, i);
					return;
				}
			}

			throw ProcessorException("Precompile: Wrong expression (do not use any spaces) (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_undefined(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() == 3)
				check_math(parts, i);
			else if (parts.size() == 1)
			{
				if (parts[0].find(":") < parts[0].size())
					check_mark(parts[0], i);
				else if (parts[0].find("(") < parts[0].size())
					check_func_call(parts[0], i);
			}
			else
				throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
		}*/

	public:
		static std::vector<std::string> compile(const std::vector<std::string> code, const Version& version)
		{
			Mova::VarVector vv;
			Mova::ConstantsVector cv;
			
			std::vector<std::string> part1;
			for (size_t i = 0; i < code.size(); ++i)
			{
				auto parts = Separator::split(Trim::left_trim(code[i]), ' ');
				if (parts.size() == 0) continue;

				auto math_parts = Mova::MathExpression::convert(parts, vv, cv, i);
				for (auto& p : math_parts) part1.push_back(p);
			}

			std::vector<std::string> res = cv.convert_to_code();
			for (auto& p : vv.convert(part1, cv.get_size()))
				res.push_back(p);

			return res;
		}
	};
}