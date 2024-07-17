#pragma once
#include <vector>
#include <string>

namespace Story
{
	using DoubleMatrix = std::vector<std::vector<double>>;

	class BaseTask
	{
	public:
		BaseTask()
			: ins({ {} }), outs({ {} }) {}

		BaseTask(const DoubleMatrix& _ins, const DoubleMatrix& _outs, const std::string& _name, const std::string& _description, const std::string& _completion_script)
			: ins(_ins), outs(_outs), name(_name), description(_description), completion_script(_completion_script) {}

		bool check_answer(const DoubleMatrix& user_outs)
		{
			if (user_outs.size() != outs.size())
				return false;
			for (int i = 0; i < outs.size(); ++i)
			{
				for (int j = 0; j < outs[i].size(); ++j)
				{
					if (std::abs(user_outs[i][j] - outs[i][j]) >= 0.001)
						return false;
				}
			}
			return true;
		}

		const DoubleMatrix& get_ins()
		{
			return ins;
		}

		const std::string& get_name()
		{
			return name;
		}

		const std::string& get_description()
		{
			return description;
		}

		const std::string& get_completion_script()
		{
			return completion_script;
		}

		virtual ~BaseTask() = default;

	protected:
		DoubleMatrix ins;
		DoubleMatrix outs;
		std::string name;
		std::string description;
		std::string completion_script;
	};
}