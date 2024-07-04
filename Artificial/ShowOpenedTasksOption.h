#pragma once
#include "AbstractControllerOption.h"
#include "TaskStateHandler.h"
#include "Parser.h"
#include "TaskHandler.h"

namespace Commands
{
	// tasks
	class ShowOpenedTasksOption final : public AbstractControllerOption
	{
	public:
		ShowOpenedTasksOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Shows all tasks and their states\n");
				ptr->print_secondary("tasks ::noclosed\n");
				ptr->print_main("  ::noclosed - (flag) hides all tasks with CLOSED state\n");
				return;
			}
	
			int i = -1;
			for (const auto& pair : Story::TaskStateHandler::s_states)
			{
				++i;
				if (pair.second.state == Story::TaskState::CLOSED && _command.has("::noclosed")) continue;
				switch (pair.second.state)
				{
				case Story::TaskState::OPEN:
					ptr->print_main(Parser::to_string(i) + ". " + pair.first + "\n");
					break;
				case Story::TaskState::COMPLETE:
					ptr->print_secondary(Parser::to_string(i) + ". " + pair.first + " (COMPLETED)\n");
					break;
				case Story::TaskState::CLOSED:
					ptr->print_third(Parser::to_string(i) + ". ------\n");
					break;
				}
			}
			
			return;
		}
	};
}