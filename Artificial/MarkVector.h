#pragma once
#include "ProcessorExceptions.h"
#include "Parser.h"
#include <map>

namespace Mova
{
	class MarkVector final
	{
	public:
		void add_mark(const std::string& name, size_t line)
		{
			if (marks.contains(name))
				throw ProcessorException("Compile: This goto mark already exists (Line " + Parser::to_string(line + 1) + ")");
			marks.emplace(name, false);
		}

		void toggle_mark(const std::string& name)
		{
			marks[name] = true;
		}

		bool is_good()
		{
			for (const auto& [name, b] : marks)
				if (!b) return false;
			return true;
		}

	private:
		std::map<std::string, bool> marks;
	};
}