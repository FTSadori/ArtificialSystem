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
	// mk "filename"
	// ::dir - create directory
	// ::sys - create system file
	// ::h - hidden
	// :p "password" - set password
	// :rp :wp :ep - read, write and execute permissions
	class MemoryCreateOption final : public AbstractControllerOption
	{
	public:
		MemoryCreateOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("Max entered permission lvl needed\n");
				ptr->print_main("Creates file or directory. Your entered lvl can't be lower than lvl in directory\n");
				ptr->print_secondary("mk {filename} [::dir] [::sys] [::h] [:p password] [:rp lvl] [:wp lvl] [:ep lvl]\n");
				ptr->print_main("  filename - (string) name of the new file;\n");
				ptr->print_main("  ::dir - (flag) create directory;\n");
				ptr->print_main("  ::sys - (flag) create system file (needs 255 lvl);\n");
				ptr->print_main("  ::h - (flag) create hidden file (needs root rights);\n");
				ptr->print_main("  :p \"password\" - (flag + string) sets up password on file/directory (works if dir doesn't have password yet);\n");
				ptr->print_main("  :rp lvl - (flag + int) sets read permission lvl on file/directory;\n");
				ptr->print_main("  :wp lvl - (flag + int) sets write permission lvl on file/directory;\n");
				ptr->print_main("  :ep lvl - (flag + int) sets execution permission lvl on file;\n");

				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			auto dir_perm = disk.get_info(path.disk_path().dir(), sender.sudo()).permissions;
			std::vector<uint8_t> all_perm_lvls = { dir_perm.write_perm_lvl };
			
			std::map<std::string, uint8_t&> perm_map = {
				{":rp", dir_perm.read_perm_lvl},
				{":wp", dir_perm.write_perm_lvl},
				{":ep", dir_perm.exec_perm_lvl}
			};

			for (const auto& [flag, link] : perm_map)
			{
				if (_command.has(flag))
				{
					auto lvl = Parser::from_string<uint8_t>(_command.get(flag));
					all_perm_lvls.push_back(lvl);
					if (link < lvl) link = lvl;
				}
			}

			std::sort(all_perm_lvls.begin(), all_perm_lvls.end());

			if (sender.lvl() < all_perm_lvls.back())
				throw PermissionException("(MemoryCreateOption) Sender has low permission lvl");

			m_core.passwords().check_password(ptr, dir_perm.password_hash);

			dir_perm.hidden |= _command.has("::h");
			
			if (_command.has(":p"))
			{
				if (dir_perm.password_hash != 0)
					throw CommandException("(MemoryCreateOption) Directory already has password");
				dir_perm.password_hash = std::hash<std::string>()(_command.get(":p"));
			}

			bool system_file = _command.has("::sys");
			if (system_file && sender.lvl() != 255)
				throw CommandException("(MemoryCreateOption) Non-system user can't create system file");

			if (_command.has("::dir"))
				disk.create(path.disk_path(), dir_perm, Memory::FileT::DIR, system_file);
			else
				disk.create(path.disk_path(), dir_perm, Memory::FileT::FILE, system_file);

			return;
		}
	};
}