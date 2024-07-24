#pragma once
#include "AbstractControllerOption.h"
#include "TaskStateHandler.h"
#include "StoryExceptions.h"
#include "Parser.h"
#include "TaskHandler.h"

namespace Commands
{
	// opentask
	class OpenTaskOption final : public AbstractControllerOption
	{
	public:
		OpenTaskOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n"); 
				ptr->print_main("Opens task for users\n");
				ptr->print_secondary("opentask {name}\n");
				ptr->print_main("  name - (string) key name of task \n");
				return;
			}

			if (!Story::TaskStateHandler::s_states.contains(_command.get("1")))
				throw Story::TaskNameException("(OpenTaskOption) TaskStateHandler doesn't know about " + _command.get("1") + " task");
			if (!Story::TaskHandler::s_tasks.contains(_command.get("1")))
				throw Story::TaskNameException("(OpenTaskOption) TaskHandler doesn't know about " + _command.get("1") + " task");

			auto& info = Story::TaskStateHandler::s_states[_command.get("1")];

			if (info.state == Story::TaskState::CLOSED)
				info.state = Story::TaskState::OPEN;

			return;
		}
	};
}