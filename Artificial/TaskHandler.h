#pragma once
#include <string>
#include "IncrementTask.h"
#include "StoryExceptions.h"
#include <memory>
#include <unordered_map>

namespace Story
{
	class TaskHandler final
	{
	public:
		static const DoubleMatrix& get_task_ins_by_name(const std::string& task)
		{
			if (!s_tasks.contains(task))
				throw TaskNameException("(TaskHandler::get_task_ins_by_name) Task with name " + task + " does not exist");
			return s_tasks[task]->get_ins();
		}

		static bool test_task_by_name(const std::string& task, const DoubleMatrix& user_outs)
		{
			if (!s_tasks.contains(task))
				throw TaskNameException("(TaskHandler::test_task_by_name) Task with name " + task + " does not exist");
			return s_tasks[task]->check_answer(user_outs);
		}

		static std::unordered_map<std::string, std::shared_ptr<BaseTask>> s_tasks;
	};
}