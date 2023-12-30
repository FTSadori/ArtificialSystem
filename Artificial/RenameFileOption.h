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
	// rename
	class RenameFileOption final : public AbstractControllerOption
	{
	public:
		RenameFileOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("File write permission lvl needed\n");
				ptr->print_main("Renames file or directory\n");
				ptr->print_secondary("rename {path} {new_name}\n");
				ptr->print_main("  path - (string) absolute or relative path;\n");
				ptr->print_main("  new_name - (string) new name of file;\n");

				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.sudo()).permissions;

			if (sender.lvl() < perm.write_perm_lvl)
				throw PermissionException("(RenameFileOption) Sender has low permission lvl");

			if (perm.hidden && !sender.sudo())
				throw PermissionException("(RenameFileOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, perm.password_hash);

			disk.rename(path.disk_path(), Memory::DiskPath(path.disk_path().dir(), _command.get("2")), sender.system());
			return;
		}
	};
}