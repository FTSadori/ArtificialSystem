#pragma once
#include "StarterTasks.h"
#include <unordered_map>
#include <string>
#include "ILoadable.h"

namespace Story
{
	enum class TaskState
	{
		CLOSED,
		OPEN,
		COMPLETE,
	};

	struct TaskInfo
	{
		TaskState state = TaskState::CLOSED;
		int totalAttempts = 0;
		int firstComplitionAttempts = -1;
		int minLinesOfCode = -1;
	};

	class TaskStateHandler : public Memory::ILoadable
	{
	public:
		static std::unordered_map<std::string, TaskInfo> s_states;

		virtual Memory::DataQueue get_as_data() const override {
			Memory::DataQueue data;
			data.push<size_t>(s_states.size());
			for (const auto& [name, info] : s_states) {
				data.push<size_t>(name.size());
				for (char c : name) 
					data.push_char(c);
				data.push<int>((int)info.state);
				data.push<int>(info.totalAttempts);
				data.push<int>(info.firstComplitionAttempts);
				data.push<int>(info.minLinesOfCode);
			}
			return data;
		}

		virtual void load_from_data(Memory::DataQueue& _data) override {
			s_states.clear();

			size_t num = _data.pop<size_t>();
			for (size_t i = 0; i < num; ++i) {
				size_t size = _data.pop<size_t>();
				std::string line = "";
				for (int j = 0; j < size; ++j)
					line += _data.pop<char>();
				TaskInfo info;
				info.state = (TaskState)_data.pop<int>();
				info.totalAttempts = _data.pop<int>();
				info.firstComplitionAttempts = _data.pop<int>();
				info.minLinesOfCode = _data.pop<int>();
				s_states[line] = info;
			}
		}
	};
}