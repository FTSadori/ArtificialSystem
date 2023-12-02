#pragma once
#include "IControllerOption.h"
#include "DiskSystem.h"
#include "FullPath.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "ICommand.h"
#include "Command.h"
#include "Parser.h"
#include <map>

namespace Commands
{
	// mk "filename"
	// ::dir - create directory
	// ::sys - create system file
	// ::h - hidden
	// :p "password" - set password
	// :rp :wp :ep - read, write and execute permissions
	class MemoryCreateOption final : public IControllerOption
	{
	public:
		MemoryCreateOption(Memory::DiskSystem& _system)
			: m_system(_system) {}

		virtual std::string execute(const ICommand& _command, const ISender& sender) override
		{
			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_system.get_disk(path.mark());
			uint8_t need = disk.get_info(path.disk_path(), sender.sudo()).permissions.write_perm_lvl;

			if (need > sender.get_lvl())
				throw PermissionException("(MemoryCreateOption) Sender has low permission lvl");

			send(Command(path.full_dir_name() + " checkpassword"));

			auto dir_perm = disk.get_info(path.disk_path().dir(), sender.sudo()).permissions;

			std::map<std::string, uint8_t&> perm_map = { 
				{":rp", dir_perm.read_perm_lvl}, 
				{":wp", dir_perm.write_perm_lvl}, 
				{":ep", dir_perm.exec_perm_lvl}
			};

			for (const auto& [flag, link] : perm_map)
			{
				if (_command.has(flag))
				{
					uint8_t rp = Parser::from_string<uint8_t>(_command.get(flag));
					if (link < rp)
						link = std::min(rp, sender.get_lvl());
				}
			}

			if (_command.has("::h")) dir_perm.hidden = true;
			
			if (_command.has(":p"))
			{
				if (dir_perm.password_hash != 0)
					throw CommandException("(MemoryCreateOption) Directory already has password");
				dir_perm.password_hash = std::hash<std::string>()(_command.get(":p"));
			}

			bool system_file = _command.has("::sys");
			if (system_file && !sender.sudo())
				throw CommandException("(MemoryCreateOption) Non-sudo user can't create system file");

			if (_command.has("::dir"))
				disk.create(path.disk_path(), dir_perm, Memory::FileT::DIR, system_file);
			else
				disk.create(path.disk_path(), dir_perm, Memory::FileT::FILE, system_file);

			return "";
		}
	
	private:
		Memory::DiskSystem& m_system;
	};
}