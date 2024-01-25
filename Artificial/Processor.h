#pragma once
#include "IProcessor.h"
#include "ProcessorExceptions.h"
#include "Version.h"

namespace Mova
{
	class Processor : public IProcessor
	{
	public:
		Processor() = default;

		virtual std::vector<float>& registers() override
		{
			return m_registers;
		}

		void set_version(const std::string& version_string)
		{
			m_version = Version(version_string);
			m_current_num_size = c_start_num_size + m_version.bits_lvl;
		}

		virtual double process(const std::vector<std::string>& code) override
		{
			m_registers.clear();
			m_registers.resize(c_registers_size, 0);
			m_execution_ptr = 0;

			while (m_execution_ptr < code.size())
			{
				int num = Parser::from_bits(code[m_execution_ptr].substr(0, c_first_bits));
				
				std::string line = code[m_execution_ptr].substr(c_first_bits);
				if (line.size() % m_current_num_size != 0)
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Wrong number of bits. Every number must be "
						+ Parser::to_string(m_current_num_size) + " bits long");
					
				auto bits_arguments = Separator::split_by(line, m_current_num_size);
				std::vector<int> arguments;
				for (const auto& bits : bits_arguments)
					arguments.push_back(Parser::from_bits(bits));

				switch (num)
				{
				case 0: zeroing(arguments); break;
				case 1: increment(arguments); break;
				case 2: transfer(arguments); break;
				case 3: jump(arguments); break;
				}

				if (num != 3)
					m_execution_ptr++;
			}
			return m_registers[0];
		}

		void check_for_arguments(const std::vector<int>& arguments, size_t min, size_t max)
		{
			if (arguments.size() < min || arguments.size() > max)
				throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Wrong number of arguments. Must be "
				      + (min == max ? Parser::to_string(min) : "from " + Parser::to_string(min) + " to " + Parser::to_string(max)));
		}

		void zeroing(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 1, 1);
			m_registers[arguments[0]] = 0;
		}

		void increment(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 1, 1);
			m_registers[arguments[0]]++;
		}

		void transfer(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 2, 2);
			m_registers[arguments[1]] = m_registers[arguments[0]];
		}

		void jump(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 3, 4);
			bool ans = false;
			int first = m_registers[arguments[0]];
			int second = m_registers[arguments[1]];
			if (arguments.size() == 4)
			{
				if ((arguments[3] == 2 || arguments[3] == 3) && m_version.jump_lvl < 1)
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
				if (arguments[3] >= 4 && m_version.jump_lvl < 2)
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
				switch (arguments[3])
				{
				case 0: ans = (first == second); break;
				case 1: ans = (first < second); break;
				case 2: ans = (first > second); break;
				case 3: ans = (first != second); break;
				case 4: ans = (first >= second); break;
				case 5: ans = (first <= second); break;
				}
			}
			else
				ans = (first == second);

			m_execution_ptr = (ans ? arguments[2] - 1 : m_execution_ptr + 1);
		}

	private:
		std::vector<float> m_registers;
		int m_stack_ptr = 0;
		int m_execution_ptr = 0;
		const uint8_t c_first_bits = 4;
		const uint8_t c_start_num_size = 4;
		const int c_registers_size = 65536;

		int m_current_num_size = 4;
		Version m_version;
	};
}