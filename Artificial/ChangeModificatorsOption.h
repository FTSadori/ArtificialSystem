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
	// chmod
	class ChangeModificatorsOption final : public AbstractControllerOption
	{
	public:
		ChangeModificatorsOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		void set_password_to_all_from(hash_t pass, Memory::DiskFileManager& disk, bool hidden, const Memory::DiskPath& path, bool system)
		{
			auto perm = disk.get_info(path, system).permissions;
			perm.password_hash = pass;
			perm.hidden = hidden;
			disk.change_info(path, perm, system);

			if (disk.get_type(path) == Memory::FileT::DIR)
			{
				const auto& list = disk.list(path, system);
				for (const auto& name : list)
				{
					set_password_to_all_from(pass, disk, hidden, Memory::DiskPath(path.full_name(), name), system);
				}
			}
		}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Max entered permission lvl needed\n");
				ptr->print_main("Changes modificators for file\n");
				ptr->print_secondary("mkmod {path} [::h] [:p password] [:rp lvl] [:wp lvl] [:ep lvl] [:preenter password]\n");
				ptr->print_main("  path - (string) file path;\n");
				ptr->print_main("  ::h - (flag) inverse hidden file settings (needs root rights);\n");
				ptr->print_main("  :p \"password\" - (flag + string) changes/sets up password on file/directory (works if dir doesn't have password yet);\n");
				ptr->print_main("  :rp lvl - (flag + int) sets read permission lvl on file/directory;\n");
				ptr->print_main("  :wp lvl - (flag + int) sets write permission lvl on file/directory;\n");
				ptr->print_main("  :ep lvl - (flag + int) sets execution permission lvl on file;\n");
				ptr->print_main("  :preenter password - (flag + string) you can enter password here if command needs it;\n");

				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto perm = disk.get_info(path.disk_path(), sender.sudo()).permissions;
			
			if (sender.lvl() < perm.write_perm_lvl)
				throw PermissionException("(ChangeModificatorsOption) Sender has low permission lvl");

			Memory::Permissions max_perm;

			if (disk.get_type(path.disk_path()) == Memory::FileT::DIR)
			{
				const auto& list = disk.list(path.disk_path(), sender.system());
				for (const auto& name : list)
				{
					auto sub_perm = disk.get_info(Memory::DiskPath(path.disk_path().full_name(), name), sender.system()).permissions;
					max_perm.read_perm_lvl = max(max_perm.read_perm_lvl, sub_perm.read_perm_lvl);
					max_perm.write_perm_lvl = max(max_perm.write_perm_lvl, sub_perm.write_perm_lvl);
				}
			}
			else
			{
				max_perm.exec_perm_lvl = sender.lvl();
				max_perm.read_perm_lvl = sender.lvl();
				max_perm.write_perm_lvl = sender.lvl();
			}

			Memory::Permissions min_perm = disk.get_info(path.disk_path().dir(), sender.system()).permissions;

			uint8_t lvl = 0;

			if (_command.has(":rp"))
			{
				lvl = (uint8_t)Parser::from_string<int>(_command.get(":rp"));
				perm.read_perm_lvl = lvl;
				if (lvl > max_perm.read_perm_lvl || lvl < min_perm.read_perm_lvl)
					throw PermissionException("(ChangeModificatorsOption) Read permission lvl is invalid");
			}
			if (_command.has(":wp"))
			{
				lvl = (uint8_t)Parser::from_string<int>(_command.get(":wp"));
				perm.write_perm_lvl = lvl;
				if (lvl > max_perm.write_perm_lvl || lvl < min_perm.write_perm_lvl)
					throw PermissionException("(ChangeModificatorsOption) Write permission lvl is invalid");
			}
			if (_command.has(":ep"))
			{
				if (disk.get_type(path.disk_path()) == Memory::FileT::FILE)
				{
					lvl = (uint8_t)Parser::from_string<int>(_command.get(":ep"));
					perm.exec_perm_lvl = lvl;
					if (lvl > max_perm.exec_perm_lvl || lvl < min_perm.exec_perm_lvl)
						throw PermissionException("(ChangeModificatorsOption) Execution permission lvl is invalid");
				}
			}
	
			if (_command.has(":preenter"))
				m_core.passwords().check_password(perm.password_hash, _command.get(":preenter"));
			else
				m_core.passwords().check_password(ptr, perm.password_hash);

			if (_command.has("::h")) perm.hidden = !perm.hidden;

			if (_command.has(":p"))
			{
				perm.password_hash = std::hash<std::string>()(_command.get(":p"));
				set_password_to_all_from(perm.password_hash, disk, perm.hidden, path.disk_path(), sender.system());
			}

			disk.change_info(path.disk_path(), perm, sender.system());

			return;
		}
	};
}