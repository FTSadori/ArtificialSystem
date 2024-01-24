#pragma once
#include "IProcessor.h"
#include "ProcessorExceptions.h"
#include "Version.h"

namespace Mova
{
	class Processor : public IProcessor
	{
	public:
		virtual std::vector<float>& registers() override
		{
			return m_registers;
		}

		void set_version(const std::string& version_string)
		{
			m_version = Version(version_string);
			m_current_num_size = c_start_num_size + m_version.bits_lvl;
		}

		virtual void process(const std::vector<std::string>& code) override
		{
			while (m_execution_ptr < code.size())
			{
				int num = Parser::from_bits(code[m_execution_ptr].substr(0, c_first_bits));
				
				std::string line = code[m_execution_ptr].substr(c_first_bits);
				if (line.size() % m_current_num_size != 0)
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Wrong number of bits. Every number must be "
						+ Parser::to_string(m_current_num_size) + " bytes long");
					
				auto bits_arguments = Separator::split_by(line, m_current_num_size);
				std::vector<int> arguments;
				for (const auto& bits : bits_arguments)
					arguments.push_back(Parser::from_bits(bits));

				switch (num)
				{
				case 0:
					break;
				default:
					break;
				}
			}
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
			m_registers[arguments[0]] = m_registers[arguments[1]];
		}

		void jump(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 3, 4);
			bool ans = false;
			if (arguments.size() == 4)
			{
				if ((arguments[3] == 2 || arguments[3] == 3) && m_version.jump_lvl < 1)
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
				if (arguments[3] >= 4 && m_version.jump_lvl < 2)
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
				switch (arguments[3])
				{
				case 0: ans = arguments[0] == arguments[1]; break;
				case 1: ans = arguments[0] < arguments[1]; break;
				case 2: ans = arguments[0] > arguments[1]; break;
				case 3: ans = arguments[0] != arguments[1]; break;
				case 4: ans = arguments[0] >= arguments[1]; break;
				case 5: ans = arguments[0] <= arguments[1]; break;
				}
			}
			else
				ans = arguments[0] == arguments[1];

			m_execution_ptr = (ans ? arguments[2] - 1 : ++m_execution_ptr);
		}

	private:
		std::vector<float> m_registers;
		int m_stack_ptr = 0;
		int m_execution_ptr = 0;
		const uint8_t c_first_bits = 4;
		const uint8_t c_start_num_size = 4;

		int m_current_num_size = c_start_num_size;
		Version m_version;
		
	};
}