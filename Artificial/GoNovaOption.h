#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "PathEventsHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// go
	class GoNovaOption final : public AbstractControllerOption
	{
	public:
		GoNovaOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("1 permission lvl needed\n");
				ptr->print_main("Moves Nova by one level up or down in file hierarchy\n");
				ptr->print_main("You need to have nova.girl file in current directory\n");
				ptr->print_secondary("go {directory}\n");
				ptr->print_main("  directory - (string) name of directory to go or .. to go back;\n");
				return;
			}

			if (sender.lvl() < 1)
				throw PermissionException("(GoNovaOption) Sender has low permission lvl");

			Memory::FullPath nova_path = Memory::RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(nova_path.mark());
			if (!disk.is_exists(nova_path.disk_path()))
				throw CommandException("(GoNovaOption) Nova is not here");

			if (_command.get("1").find('\\') != std::string::npos)
				throw CommandException("(GoNovaOption) You can use only directories in current directory or '..' literal to go back");

			Memory::FullPath new_path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));
			if (disk.get_type(new_path.disk_path()) != Memory::FileT::DIR)
				throw CommandException("(GoNovaOption) " + _command.get("1") + " is not a directory");

			//
			auto& disk2 = m_core.memory().get_disk(new_path.mark());
			auto perm = disk2.get_info(new_path.disk_path(), sender.system()).permissions;

			if (sender.lvl() < perm.read_perm_lvl)
				throw PermissionException("(GoNovaOption) Sender has low permission lvl");

			if (new_path.disk_path().full_name() != "" && perm.hidden && !sender.sudo())
				throw PermissionException("(GoNovaOption) Sender has low permission lvl");
			//

			Command cmd("\"" + _command.get("path") + "\" cd \"" + new_path.full_path_str() + "\"");
			m_core.execute(cmd, User("system", true, 255));

			disk.move(nova_path.disk_path(), new_path.disk_path(), sender.system());

			using namespace Memory;
			std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
			auto& maindisk = m_core.memory().get_disk(mainmark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.goevent")))
				maindisk.create(DiskPath("\\system\\.goevent"), Permissions(true, 255, 255, 255, 0), FileT::FILE, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.save")))
				maindisk.create(DiskPath("\\system\\.save"), Permissions(true, 255, 255, 255, 0), FileT::FILE, true);

			if (!Story::PathEventsHandler::is_loaded())
			{
				auto data = maindisk.read(DiskPath("\\system\\.goevent"), true);
				Story::PathEventsHandler::load(std::string(data.get_data(), data.size()));
			}

			FullPath str_path = RelativePathCreator::combine(_command.get("path"), _command.get("1"));
			const auto& str = Story::PathEventsHandler::get(str_path.full_path_str());
			if (str != "")
			{
				Command cmd("\"" + ptr->get_path().full_path_str() + "\" run \"" + str + "\"");
				m_core.execute(cmd, User("amogus", true, 255));
			}

			Command cmdsave("\"" + ptr->get_path().full_path_str() + "\" write \"" + mainmark + ":\\system\\.save\" \"" + ptr->get_path().full_path_str() + "\" ::nobase64");
			m_core.execute(cmdsave, User("amogus", true, 255));

			return;
		}
	};
}