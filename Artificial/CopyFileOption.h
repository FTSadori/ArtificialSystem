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
	// copy
	class CopyFileOption final : public AbstractControllerOption
	{
	public:
		CopyFileOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("File write permission lvl needed\n");
				ptr->print_main("Copy file or directory to another path\n");
				ptr->print_secondary("copy {path} {new_path}\n");
				ptr->print_main("  path - (string) absolute or relative path;\n");
				ptr->print_main("  new_path - (string) destination path (you should add new file name);\n");

				return;
			}

			// todo fix copypast
			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));
			Memory::FullPath new_path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("2"));

			if (Story::BannedNamesValidator::is_banned(new_path.disk_path().file()))
				throw CommandException("(CopyFileOption) New file name is banned. Use story commands to create files with banned names");

			auto& disk = m_core.memory().get_disk(path.mark());
			auto& new_disk = m_core.memory().get_disk(new_path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.system()).permissions;
			auto new_perm = new_disk.get_info(new_path.disk_path().dir(), sender.system()).permissions;

			if (sender.lvl() < perm.write_perm_lvl || sender.lvl() < new_perm.write_perm_lvl)
				throw PermissionException("(CopyFileOption) Sender has low permission lvl");

			if (perm.hidden && new_perm.hidden && !sender.sudo())
				throw PermissionException("(CopyFileOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, perm.password_hash, "Input password for source: ");
			m_core.passwords().check_password(ptr, new_perm.password_hash, "Input password for destination: ");

			if (path.mark() == new_path.mark())
			{
				disk.copy(path.disk_path(), new_path.disk_path(), sender.system());
			}
			else
			{
				m_core.memory().copy(path, new_path, sender.system());
			}
			return;
		}
	};
}