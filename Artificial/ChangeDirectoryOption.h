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
				ptr->print_secondary("cd {path}\n");
				ptr->print_main("  path - (string) absolute or relative path;\n");
				
				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.sudo()).permissions;
			
			if (sender.lvl() < perm.read_perm_lvl)
				throw PermissionException("(MemoryCreateOption) Sender has low permission lvl");

			if (perm.hidden && !sender.sudo())
				throw PermissionException("(MemoryCreateOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, perm.password_hash);

			ptr->set_path(path);

			return;
		}
	};
}