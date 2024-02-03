#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "ProcessorExceptions.h"
#include "FinalModuleKeyWords.h"
#include "Parser.h"
#include "Trim.h"
#include "Separator.h"

namespace Mova
{
	class VarVector final
	{
	public:
		VarVector()
		{
			m_vars.emplace_back();
		}

		void clear()
		{
			m_vars.clear();
			m_cur_base = 0;
			m_cur_nesting_lvl = 0;
		}

		size_t get_ptr_by_name(const std::string& name, size_t line)
		{
			for (int j = m_cur_nesting_lvl; j >= 0; --j)
			{
				if (m_vars[j].contains(name))
					return m_vars[j][name];
			}

			throw ProcessorException("Precompile: Undefined variable (Line " + Parser::to_string(line + 1) + ")");
		}

		void push_var(const std::string& name, size_t line)
		{
			if (name.size() == 0 || isdigit(name[0]))
				throw ProcessorException("Precompile: Veriable name cannot begin with digit (Line " + Parser::to_string(line + 1) + ")");

			for (char c : name)
				if (!isalnum(c) && c != '-' && c != '_')
					throw ProcessorException("Precompile: Variable name must contain only digits, latin letters, '-' and '_' (Line " + Parser::to_string(line + 1) + ")");
			
			if (FinalModuleKeyWords::is_keyword(name))
				throw ProcessorException("Precompile: Wrong used keyword (Line " + Parser::to_string(line + 1) + ")");

			if (!m_vars.back().contains(name))
				m_vars.back().emplace(name, m_cur_base + m_vars.back().size());
		}

		void push_layer()
		{
			m_cur_base += m_vars.back().size();
			m_vars.emplace_back();
		}

		void pop_layer()
		{
			m_vars.pop_back();
			m_cur_base -= m_vars.back().size();
		}

		std::vector<std::string> convert(const std::vector<std::string>& code, size_t from)
		{
			std::vector<std::string> res;
			for (const auto& line : code)
			{
				if (line.find('v') >= line.size())
				{
					res.push_back(line);
					continue;
				}
			
				auto arr = Separator::split(line, ' ');
				std::vector<std::string> new_arr;
				for (const auto& part : arr)
				{
					if (part.size() > 0 && part[0] == 'v')
					{
						size_t ptr = from + Parser::from_string<size_t>(part.substr(1));
						new_arr.push_back(Parser::to_string(ptr));
					}
					else
						new_arr.push_back(part);
				}

				res.push_back(Separator::join(new_arr, ' '));
			}
			return res;
		}

	private:
		std::vector<std::unordered_map<std::string, size_t>> m_vars;
		int m_cur_nesting_lvl = 0;
		size_t m_cur_base = 0;
	};
}