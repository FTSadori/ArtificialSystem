#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// mova
	class RunProgramOption final : public AbstractControllerOption
	{
	public:
		RunProgramOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("R permission lvl needed\n");
				ptr->print_main("Runs program using MOVA processor\n");
				ptr->print_secondary("mova {path}\n");
				ptr->print_main("  path - (string) path to file with code;\n");
				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;

			if (disk.get_type(path.disk_path()) != Memory::FileT::FILE)
				throw CommandException("(RunProgramOption) It's not a file");

			if (sender.lvl() < perm.read_perm_lvl)
				throw PermissionException("(RunProgramOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, perm.password_hash);

			auto data = disk.read(path.disk_path(), sender.system());

			auto lines = Separator::split(std::string(data.get_data(), data.size()), '\n');

			// currently we have only first module
			m_core.processor().set_version(lines[0]);
			lines.erase(lines.begin());
			double ans = m_core.processor().process(lines);
			ptr->print_third("Program returned " + Parser::to_string(ans) + "\n");

			return;
		}
	};
}