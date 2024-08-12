#pragma once
#include "AbstractControllerOption.h"
#include "TaskStateHandler.h"
#include "StoryExceptions.h"
#include "Parser.h"
#include "TaskHandler.h"

namespace Commands
{
	// showtask
	class ShowTaskDetailsOption final : public AbstractControllerOption
	{
	public:
		ShowTaskDetailsOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("13 permission level needed\n");
				ptr->print_main("Shows information about task with entered name\n");
				ptr->print_secondary("showtask {name}\n");
				ptr->print_main("  name - (string) key name of task \n");
				return;
			}

			if (sender.lvl() < 13)
				throw PermissionException("(ShowTaskDetailsOption) Sender has low permission lvl");

			if (!Story::TaskStateHandler::s_states.contains(_command.get("1")))
				throw Story::TaskNameException("(ShowTaskDetailsOption) TaskStateHandler doesn't know about " + _command.get("1") + " task");
			if (!Story::TaskHandler::s_tasks.contains(_command.get("1")))
				throw Story::TaskNameException("(ShowTaskDetailsOption) TaskHandler doesn't know about " + _command.get("1") + " task");

			const auto& info = Story::TaskStateHandler::s_states[_command.get("1")];

			switch (info.state)
			{
			case Story::TaskState::OPEN:
				ptr->print_main("State: OPEN\n");
				break;
			case Story::TaskState::COMPLETE:
				ptr->print_secondary("State: COMPLETE\n");
				break;
			case Story::TaskState::CLOSED:
				ptr->print_third("State: CLOSED\n");
				break;
			case Story::TaskState::SKIPPED:
				ptr->print_third("State: SKIPPED\n");
				break;
			}

			if (info.state == Story::TaskState::CLOSED)
			{
				ptr->print_main("Name: ------\n");
				ptr->print_main("Description: ------\n");
				ptr->print_main("First complition attempts: ------\n");
				ptr->print_main("All attempts: ------\n");
				ptr->print_main("Min file length: ------\n");
				return;
			}
			
			const auto& ptr_to_task = Story::TaskHandler::s_tasks[_command.get("1")];
			ptr->print_main("Name: " + ptr_to_task->get_name() + "\n");
			ptr->print_main("Description: " + ptr_to_task->get_description() + "\n");
			ptr->print_main("First complition attempts: " + Parser::to_string(info.firstComplitionAttempts) + "\n");
			ptr->print_main("All attempts: " + Parser::to_string(info.totalAttempts) + "\n");
			ptr->print_main("Min file length: " + Parser::to_string(info.minLinesOfCode) + "\n");

			return;
		}
	};
}