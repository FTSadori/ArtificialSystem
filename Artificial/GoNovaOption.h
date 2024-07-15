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
				ptr->print_main("No permission lvl needed\n");
				ptr->print_main("Moves Nova by one level up or down in file hierarchy\n");
				ptr->print_main("You need to have nova.girl file in current directory\n");
				ptr->print_secondary("go {directory}\n");
				ptr->print_main("  directory - (string) name of directory to go or .. to go back;\n");
				return;
			}

			Memory::FullPath nova_path = Memory::RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(nova_path.mark());
			if (!disk.is_exists(nova_path.disk_path()))
				throw CommandException("(GoNovaOption) Nova is not here");

			if (_command.get("1").find('\\') != std::string::npos)
				throw CommandException("(GoNovaOption) You can use only directories in current directory or '..' literal to go back");

			Memory::FullPath new_path = Memory::RelativePathCreator::combine(_command.get("path"), _command.get("1"));
			if (disk.get_type(new_path.disk_path()) != Memory::FileT::DIR)
				throw CommandException("(GoNovaOption) " + _command.get("1") + " is not a directory");

			Command cmd("\"" + _command.get("path") + "\" cd \"" + new_path.full_path_str() + "\"");
			m_core.execute(cmd, sender);

			disk.move(nova_path.disk_path(), new_path.disk_path(), sender.system());

			return;
		}
	};
}