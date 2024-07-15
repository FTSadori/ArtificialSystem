#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include <map>

namespace Commands
{
	// cd
	class ChangeDirectoryOption final : public AbstractControllerOption
	{
	public:
		ChangeDirectoryOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Different permission lvl needed\n");
				ptr->print_main("Moves you to entered directory\n");
				ptr->print_secondary("cd {path} [:preenter password]\n");
				ptr->print_main("  path - (string) absolute or relative path;\n");
				ptr->print_main("  :preenter password - (flag + string) you can enter password here if command needs it;\n");

				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;
			
			if (disk.get_type(path.disk_path()) == Memory::FileT::FILE)
				throw CommandException("(ChangeDirectoryOption) It's not a directory");

			if (sender.lvl() < perm.read_perm_lvl)
				throw PermissionException("(ChangeDirectoryOption) Sender has low permission lvl");

			if (path.disk_path().full_name() != "" && perm.hidden && !sender.sudo())
				throw PermissionException("(ChangeDirectoryOption) Sender has low permission lvl");

			if (_command.has(":preenter"))
				m_core.passwords().check_password(perm.password_hash, _command.get(":preenter"));
			else
				m_core.passwords().check_password(ptr, perm.password_hash);

			ptr->set_path(path);

			return;
		}
	};
}