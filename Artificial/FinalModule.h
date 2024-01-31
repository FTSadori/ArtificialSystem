#pragma once
#include "Version.h"
#include <unordered_map>
#include "ProcessorExceptions.h"
#include "Separator.h"
#include <stack>
#include "Trim.h"

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
		static std::unordered_map<std::string, size_t> s_vars;
		static std::unordered_map<std::string, size_t> s_marks;
		static std::unordered_map<std::string, FuncInfo> s_functions;

		static const std::vector<std::string> keywords;
		static const std::vector<std::string> math_expr;
		static const std::vector<std::string> comparison_expr;

		static void precompile(const std::vector<std::string>& code)
		{
			s_constants.clear();
			s_vars.clear();
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
		}

		static void check_size_t(const std::string& word, size_t i)
		{
			try
			{
				Parser::from_string<size_t>(word);
			}
			catch (...)
			{
				throw ProcessorException("Precompile: Wrong address value (Line " + Parser::to_string(i + 1) + ")");
			}
		}

		static void check_double(const std::string& word, size_t i)
		{
			try
			{
				double d = Parser::from_string<double>(word);
				if (!s_constants.contains(d))
					s_constants.emplace(d, s_constants.size());
			}
			catch (...)
			{
				throw ProcessorException("Precompile: Wrong double (Line " + Parser::to_string(i + 1) + ")");
			}
		}

		static void check_var(const std::string& word, size_t i, bool check = true)
		{
			for (char c : std::string("()&*:"))
			{
				if (word.find(c) < word.size())
					throw ProcessorException("Precompile: Variable name cannot contain '(', ')', '&', '*' or ':' (Line " + Parser::to_string(i + 1) + ")");
			}
			if (std::find(keywords.begin(), keywords.end(), word) == keywords.end())
				throw ProcessorException("Precompile: Wrong used keyword (Line " + Parser::to_string(i + 1) + ")");
			if (check && !s_vars.contains(word))
				throw ProcessorException("Precompile: Undefined variable (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_ampersant(const std::string& word, size_t i)
		{
			auto ptr = word.substr(1);
			if (ptr.size() == 0)
				throw ProcessorException("Precompile: Wrong '&' (Line " + Parser::to_string(i + 1) + ")");
			if (isalpha(ptr[0]))
			{
				check_var(ptr, i);
			}
			else
				throw ProcessorException("Precompile: What the duck is this? (Line " + Parser::to_string(i + 1) + ")");
		}
		
		static void check_asterisk(const std::string& word, size_t i)
		{
			auto ptr = word.substr(1);
			if (ptr.size() == 0)
				throw ProcessorException("Precompile: Wrong '*' (Line " + Parser::to_string(i + 1) + ")");
			if (isalpha(ptr[0]))
			{
				check_var(ptr, i);
			}
			else if (isdigit(ptr[0]))
			{
				check_size_t(ptr, i);
			}
			else
				throw ProcessorException("Precompile: What the duck is this? (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_rvalue(const std::string& word, size_t i)
		{
			if (word.size() == 0)
				throw ProcessorException("Precompile: Empty rvalue (Line " + Parser::to_string(i + 1) + ")");
			if (word.starts_with("*"))
			{
				check_asterisk(word, i);
			}
			if (word.starts_with("&"))
			{
				check_ampersant(word, i);
			}
			if (isalpha(word[0]))
			{
				check_var(word, i);
			}
			else if (isdigit(word[0]))
			{
				check_double(word, i);
			}
			else
				throw ProcessorException("Precompile: What the duck is this? (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_lvalue(const std::string& word, size_t i)
		{
			if (word.size() == 0)
				throw ProcessorException("Precompile: Empty lvalue (Line " + Parser::to_string(i + 1) + ")");
			if (word.starts_with("*"))
			{
				check_asterisk(word, i);
			}
			if (isalpha(word[0]))
			{
				check_var(word, i, false);
			}
			else
				throw ProcessorException("Precompile: Wrong lvalue (Line " + Parser::to_string(i + 1) + ")");
		}

		static void check_math(const std::vector<std::string>& parts, size_t i)
		{
			if (parts.size() != 3 ||
				std::find(math_expr.begin(), math_expr.end(), parts[1]) == math_expr.end())
				throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
			check_lvalue(parts[0], i);
			check_rvalue(parts[2], i);
			if (parts[1] == "=" && !s_vars.contains(parts[0]))
				s_vars.emplace(parts[0], s_vars.size());
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
				if (!s_vars.contains(parts[1]))
					s_vars.emplace(parts[1], s_vars.size());
			}

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
						auto args_sep = Separator::split(second_sep[1].substr(0, second_sep[1].size()), ',');
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

			throw ProcessorException("Precompile: Wrong expression (Line " + Parser::to_string(i + 1) + ")");
		}

	public:
		static std::vector<std::string> compile(const std::vector<std::string> code, const Version& version)
		{
			precompile(code);
			std::vector<std::string> res;
			size_t i = 0;
			while (i < code.size())
			{
				std::string line = Trim::left_trim(code[i]);
				
			}
			return res;
		}
	};
}