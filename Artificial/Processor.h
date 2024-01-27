#pragma once
#include "IProcessor.h"
#include "ProcessorExceptions.h"
#include "Version.h"
#include <stack>

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

		virtual std::vector<float>& process(const std::vector<std::string>& code, const std::vector<double>& input) override
		{
			m_call_stack = std::stack<int>();
			m_register_points = std::stack<int>();
			m_registers.clear();
			m_registers.resize(c_registers_size, 0);
			m_execution_ptr = 0;
			m_func_mode = false;

			auto max_num = pow(2, m_current_num_size) - 1;
			if (code.size() > max_num)
				throw ProcessorException("Code: File can't have more than " + Parser::to_string(max_num) + " rows");

			for (size_t i = 0; i < input.size(); ++i)
			{
				if (input[i] > max_num)
					throw ProcessorException("Input: Input can't be larger than " + Parser::to_string(m_current_num_size) + " bits");
				switch (m_version.types_lvl)
				{
				case 0: m_registers[i] = round(abs(input[i])); break;
				case 1: m_registers[i] = round(input[i]); break;
				case 2: m_registers[i] = input[i]; break;
				}
			}

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
				case 0b0000: zeroing(arguments); break;
				case 0b0001: increment(arguments); break;
				case 0b0010: transfer(arguments); break;
				case 0b0011: jump(arguments); break;
				case 0b0100: decrement(arguments); break;
				case 0b0101: math(arguments, '+', 1); break;
				case 0b0110: math(arguments, '-', 1); break;
				case 0b0111: math(arguments, '*', 2); break;
				case 0b1000: math(arguments, 'd', 2); break;
				case 0b1001: math(arguments, '%', 2); break;
				case 0b1010: math(arguments, '/', 2); break;

				case 0b1110: func(arguments); break;
				case 0b1111: end(arguments); break;
				}

				if (num != 3)
					m_execution_ptr++;
			}

			if (m_func_mode)
				throw ProcessorException("Code: Function didn't found END (1111) command");

			return m_registers;
		}

		void check_for_arguments(const std::vector<int>& arguments, size_t min, size_t max)
		{
			if (arguments.size() < min || arguments.size() > max)
				throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Wrong number of arguments. Must be "
				      + (min == max ? Parser::to_string(min) : "from " + Parser::to_string(min) + " to " + Parser::to_string(max)));
		}

		int get_index(int num)
		{
			if (m_func_mode)
			{
				if (num >= pow(2, m_current_num_size - 1)) num -= pow(2, m_current_num_size);
				if (m_register_points.top() + num < 0 || m_register_points.top() + num >= c_registers_size)
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Index out of range");
				return m_register_points.top() + num;
			}
			else
				return num;
		}

		void zeroing(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 1, 1);
			m_registers[get_index(arguments[0])] = 0;
			m_last_register = max(m_last_register, arguments[0]);
		}

		void increment(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 1, 1);
			m_registers[get_index(arguments[0])]++;
			m_last_register = max(m_last_register, get_index(arguments[0]));
		}

		void transfer(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 2, 2);
			m_registers[get_index(arguments[1])] = m_registers[get_index(arguments[0])];
			m_last_register = max(m_last_register, get_index(arguments[1]));
		}

		void jump(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 3, 4);
			bool ans = false;
			int first = m_registers[get_index(arguments[0])];
			int second = m_registers[get_index(arguments[1])];
			if (arguments.size() == 4)
			{
				if ((arguments[3] == 1 || arguments[3] == 2) && m_version.jump_lvl < 1)
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
				if (arguments[3] >= 3 && m_version.jump_lvl < 2)
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

		void decrement(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 1, 1);
			if (m_version.types_lvl < 1)
				throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
			m_registers[get_index(arguments[0])]--;
			m_last_register = max(m_last_register, get_index(arguments[0]));
		}

		void math(const std::vector<int>& arguments, char symb, int needed_base_lvl)
		{
			check_for_arguments(arguments, 2, 3);
			if (m_version.math_lvl < needed_base_lvl || (arguments.size() == 3 && m_version.math_lvl < 3))
				throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
			auto second = (arguments.size() == 3 ? m_registers[get_index(arguments[1])] : arguments[1]);
			switch (symb)
			{
			case '+': m_registers[get_index(arguments[0])] += second; break;
			case '-': m_registers[get_index(arguments[0])] -= second; break;
			case '*': m_registers[get_index(arguments[0])] *= second; break;
			case 'd': m_registers[get_index(arguments[0])] = round(m_registers[get_index(arguments[0])] / second); break;
			case '%': m_registers[get_index(arguments[0])] = int(m_registers[get_index(arguments[0])]) % int(second); break;
			case '/': 
				if (m_version.types_lvl >= 2)
					m_registers[get_index(arguments[0])] /= second; 
				else
					throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
			break;
			}
			m_last_register = max(m_last_register, get_index(arguments[0]));
		}

		void func(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 1, 2);
			if (m_version.func_lvl < 1 || (m_version.func_lvl < 2 && arguments.size() == 2))
				throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
			if (arguments.size() == 2)
			{
				m_call_stack.push(m_execution_ptr + 1);
				m_register_points.push(m_last_register + 1);
				m_execution_ptr = arguments[1];
				m_func_mode = true;
			}
			else
			{
				// add necessary functions
			}
		}

		void end(const std::vector<int>& arguments)
		{
			check_for_arguments(arguments, 0, 0);
			if (m_version.func_lvl < 1)
				throw ProcessorException("Line " + Parser::to_string(m_execution_ptr + 1) + ": Low MOVA level");
			m_call_stack.pop();
			m_last_register = m_register_points.top();
			m_register_points.pop();
			if (m_call_stack.empty())
				m_func_mode = false;
		}

	private:
		std::vector<float> m_registers;
		int m_stack_ptr = 0;
		int m_execution_ptr = 0;
		const uint8_t c_first_bits = 4;
		const uint8_t c_start_num_size = 4;
		const int c_registers_size = 65536;

		int m_last_register = 0;

		bool m_func_mode = false;

		std::stack<int> m_call_stack;
		std::stack<int> m_register_points;

		int m_current_num_size = 4;
		Version m_version;
	};
}