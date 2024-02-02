#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include "ProcessorExceptions.h"
#include "FinalModuleKeyWords.h"
#include "Parser.h"

namespace Mova
{
	class VarVector final
	{
	public:
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
			for (char c : name)
				if (!isalnum(c) && c != '-' && c != '_')
					throw ProcessorException("Precompile: Variable name must contain only digits, latin letters, '-' and '_' (Line " + Parser::to_string(line + 1) + ")");
			
			if (FinalModuleKeyWords::is_keyword(name))
				throw ProcessorException("Precompile: Wrong used keyword (Line " + Parser::to_string(line + 1) + ")");

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

	private:
		std::vector<std::unordered_map<std::string, size_t>> m_vars;
		int m_cur_nesting_lvl = 0;
		size_t m_cur_base = 0;
	};
}