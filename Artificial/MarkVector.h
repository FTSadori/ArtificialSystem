#pragma once
#include "ProcessorExceptions.h"
#include "Parser.h"
#include <map>
#include <set>

namespace Mova
{
	class MarkVector final
	{
	public:
		void add_mark(const std::string& name, size_t code_size, size_t line)
		{
			if (marks.contains(name))
				throw ProcessorException("Compile: This goto mark already exists (Line " + Parser::to_string(line + 1) + ")");
			marks.emplace(name, code_size);
		}

		void meet_goto(const std::string& name, size_t line)
		{
			gotos.insert(name);
		}

		size_t return_line(const std::string& name)
		{
			return marks.at(name);
		}

		bool check()
		{
			for (const auto& name : gotos)
				if (!marks.contains(name))
					throw ProcessorException("Compile: " + name + " mark was not defined");
			return true;
		}

		std::vector<std::string> convert(const std::vector<std::string>& code, size_t cv_code_size)
		{
			check();

			std::vector<std::string> res;
			for (const auto& line : code)
			{
				if (line.find('m') >= line.size())
				{
					res.push_back(line);
					continue;
				}

				auto arr = Separator::split(line, ' ');
				std::vector<std::string> new_arr;
				for (const auto& part : arr)
				{
					if (part.size() > 0 && part[0] == 'm')
					{
						size_t ptr = cv_code_size + return_line(part.substr(1)) + 1;
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
		std::set<std::string> gotos;
		std::map<std::string, size_t> marks;
	};
}