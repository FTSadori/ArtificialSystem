#pragma once
#include <vector>

namespace Story
{
	class BaseTask
	{
	public:
		BaseTask(const std::vector<std::vector<float>>& _ins, const std::vector<std::vector<float>>& _outs)
			: ins(_ins), outs(_outs) {}

		bool checkAnswer(const std::vector<std::vector<float>>& user_outs)
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

		const std::vector<std::vector<float>> ins;
	protected:
		virtual ~BaseTask() = default;

		const std::vector<std::vector<float>> outs;
	};
}