#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RemoveHandler.h"
#include <map>

namespace Commands
{
	// rm
	class MemoryDeleteOption final : public AbstractControllerOption
	{
	public:
		MemoryDeleteOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		uint8_t find_max_lvl(Memory::DiskFileManager& disk, Memory::DiskPath path, bool system)
		{
			uint8_t lvl = disk.get_info(path, system).permissions.write_perm_lvl;

			if (disk.get_type(path) == Memory::FileT::DIR)
			{
				for (const auto& name : disk.list(path, system))
				{
					uint8_t sub_lvl = find_max_lvl(disk, Memory::DiskPath(path.full_name(), name), system);
					lvl = max(lvl, sub_lvl);
				}
			}
			return lvl;
		}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("From 99 permission lvl needed\n");
				ptr->print_main("Deletes file or directory and its content. Your permission lvl must be higher or equal to max write lvl in folder.\n");
				ptr->print_secondary("rm {filename}\n");
				ptr->print_main("  filename - (string) name of the new file;\n");
				
				return;
			}

			Memory::FullPath path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));

			auto& disk = m_core.memory().get_disk(path.mark());
			
			if (sender.lvl() < 99)
				throw PermissionException("(MemoryDeleteOption) Sender has low permission lvl");

			if (Story::RemoveHandler::s_map.contains(path.full_path_str()))
			{
				Command cmd("\"" + _command.get("path") + "\" run \"" + Story::RemoveHandler::s_map.at(path.full_path_str()) + "\"");
				m_core.execute(cmd, User("hand of god", true, 255));
			}

			if (sender.lvl() < find_max_lvl(disk, path.disk_path(), sender.system()))
				throw PermissionException("(MemoryDeleteOption) Sender has low permission lvl");

			disk.remove(path.disk_path(), sender.system());
			
			return;
		}
	};
}