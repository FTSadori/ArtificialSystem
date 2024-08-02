#pragma once
#include "AbstractControllerOption.h"
#include "TaskStateHandler.h"
#include "StoryExceptions.h"
#include "Parser.h"
#include "TaskHandler.h"

namespace Commands
{
	// skiptask
	class SkipTaskOption final : public AbstractControllerOption
	{
	public:
		SkipTaskOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Ne permission lvl needed\n");
				ptr->print_main("Skip task\n");
				ptr->print_secondary("skiptask {name} {endgamepassword}\n");
				ptr->print_main("  name - (string) key name of task \n");
				ptr->print_main("  endgamepassword - (string) password given to you after first-time complition \n");
				return;
			}

			if (!Story::TaskStateHandler::s_states.contains(_command.get("1")))
				throw Story::TaskNameException("(SkipTaskOption) TaskStateHandler doesn't know about " + _command.get("1") + " task");
			if (!Story::TaskHandler::s_tasks.contains(_command.get("1")))
				throw Story::TaskNameException("(SkipTaskOption) TaskHandler doesn't know about " + _command.get("1") + " task");
			if (_command.get("2") != "1h4nk_y0u")
				throw Story::TaskNameException("(SkipTaskOption) Wrong password");

			auto& info = Story::TaskStateHandler::s_states[_command.get("1")];

			if (info.state == Story::TaskState::OPEN)
				info.state = Story::TaskState::SKIPPED;

			const auto& task_ptr = Story::TaskHandler::s_tasks[_command.get("1")];

			Command cmd("\"" + ptr->get_path().full_path_str() + "\" run \"" + task_ptr->get_completion_script() + "\"");
			m_core.execute(cmd, User("amogus", true, 255));
				
			using namespace Memory;

			auto& maindisk = m_core.memory().get_disk(m_core.memory_info().get_main_disk_info().mark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.taskstates")))
				maindisk.create(DiskPath("\\system\\.taskstates"), Permissions(true, 255, 255, 255, 0), FileT::FILE, true);

			DataQueue data2 = Story::TaskStateHandler().get_as_data();
			maindisk.write(DiskPath("\\system\\.taskstates"), data2, true);

			return;
		}
	};
}