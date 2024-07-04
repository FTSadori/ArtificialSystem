#pragma once
#include <vector>
#include <string>

namespace Story
{
	using FloatMatrix = std::vector<std::vector<float>>;

	class BaseTask
	{
	public:
		BaseTask()
			: ins({ {} }), outs({ {} }) {}

		BaseTask(const FloatMatrix& _ins, const FloatMatrix& _outs, const std::string& _name, const std::string& _description)
			: ins(_ins), outs(_outs), name(_name), description(_description) {}

		bool check_answer(const FloatMatrix& user_outs)
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

		const FloatMatrix& get_ins()
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

		virtual ~BaseTask() = default;

	protected:
		FloatMatrix ins;
		FloatMatrix outs;
		std::string name;
		std::string description;
	};
}