#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "HexModule.h"
#include "SymbolsModule.h"
#include "FinalModule.h"
#include "TaskHandler.h"
#include "TaskStateHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// runtests
	class RunTestsOption final : public AbstractControllerOption
	{
	public:
		RunTestsOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Read permission lvl needed\n");
				ptr->print_main("Runs program using MOVA processor\n");
				ptr->print_secondary("runtests {path} {name} [:preenter password]\n");
				ptr->print_main("  path - (string) path to file with code;\n");
				ptr->print_main("  name - (string) task name \n");
				ptr->print_main("  :preenter password - (flag + string) you can enter password here if command needs it;\n");
				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;

			if (disk.get_type(path.disk_path()) != Memory::FileT::FILE)
				throw CommandException("(RunTestsOption) It's not a file");

			if (sender.lvl() < perm.read_perm_lvl)
				throw PermissionException("(RunTestsOption) Sender has low permission lvl");

			if (!Story::TaskStateHandler::s_states.contains(_command.get("2")))
				throw Story::TaskNameException("(RunTestsOption) TaskStateHandler doesn't know about " + _command.get("2") + " task");

			if (!Story::TaskHandler::s_tasks.contains(_command.get("2")))
				throw Story::TaskNameException("(RunTestsOption) TaskHandler doesn't know about " + _command.get("2") + " task");

			if (_command.has(":preenter"))
				m_core.passwords().check_password(perm.password_hash, _command.get(":preenter"));
			else
				m_core.passwords().check_password(ptr, perm.password_hash);

			auto data = disk.read(path.disk_path(), sender.system());

			auto lines = Separator::split(std::string(data.get_data(), data.size()), '\n');

			Mova::Version ver = lines[0];
			m_core.processor().set_version(ver);
			lines.erase(lines.begin());

			int lines_num = lines.size();

			auto num_size = m_core.processor().get_current_num_size();
			switch (ver.module_lvl)
			{
			case 2: lines = Mova::HexModule::compile(lines); break;
			case 3: lines = Mova::SymbolsModule::compile(lines, num_size); break;
			case 4:
				auto l = Mova::FinalModule::compile(lines, ver);
				if (_command.has("::show"))
					for (auto& line : l)
						ptr->print_third(line + "\n");
				lines = Mova::SymbolsModule::compile(l, num_size);
				break;
			}

			const auto& task_ptr = Story::TaskHandler::s_tasks[_command.get("2")];
			auto& task_info = Story::TaskStateHandler::s_states[_command.get("2")];

			Story::DoubleMatrix matrix;
			for (const auto& in : task_ptr->get_ins())
			{
				matrix.emplace_back(m_core.processor().process(lines, in));
			}


			bool letsgo = false;
			task_info.totalAttempts += 1;
			if (task_ptr->check_answer(matrix))
			{
				ptr->print_third("SUCCESS!!!\n");
				task_info.state = Story::TaskState::COMPLETE;
				task_info.firstComplitionAttempts = min(task_info.firstComplitionAttempts, task_info.totalAttempts);
				if (task_info.minLinesOfCode == -1)
				{
					letsgo = true;
					task_info.minLinesOfCode = lines_num;
				}
				else
					task_info.minLinesOfCode = min(lines_num, task_info.minLinesOfCode);
			}
			else
			{
				ptr->print_third("FAILED...\n");
			}
			
			using namespace Memory;

			auto& maindisk = m_core.memory().get_disk(m_core.memory_info().get_main_disk_info().mark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.taskstates")))
				maindisk.create(DiskPath("\\system\\.taskstates"), Permissions(true, 255, 255, 255, 0), FileT::FILE, true);

			DataQueue data2 = Story::TaskStateHandler().get_as_data();
			maindisk.write(DiskPath("\\system\\.taskstates"), data2, true);

			if (letsgo)
			{
				Command cmd("\"" + ptr->get_path().full_path_str() + "\" run \"" + task_ptr->get_completion_script() + "\"");
				m_core.execute(cmd, User("amogus", true, 255));
			}

			return;
		}
	};
}