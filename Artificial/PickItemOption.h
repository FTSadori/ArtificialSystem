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
	// pick
	class PickItemOption final : public AbstractControllerOption
	{
	public:
		PickItemOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("2 permission lvl needed\n");
				ptr->print_main("Moves chosen item file to Nova's inventory\n");
				ptr->print_main("You need to have nova.girl file in current directory\n");
				ptr->print_main("Works only with *.item files\n");
				ptr->print_secondary("pick {itemfile}\n");
				ptr->print_main("  itemfile - (string) name of file to pick up;\n");
				return;
			}

			using namespace Memory;

			if (sender.lvl() < 2)
				throw PermissionException("(PickItemOption) Sender has low permission lvl");

			FullPath nova_path = RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(nova_path.mark());
			if (!disk.is_exists(nova_path.disk_path()))
				throw CommandException("(PickItemOption) Nova is not here");

			std::string itemfile = _command.get("1") + ".item";

			if (itemfile.find('\\') != std::string::npos)
				throw CommandException("(PickItemOption) You can use only files in current directory");

			FullPath new_path = RelativePathCreator::combine(_command.get("path"), itemfile);
			if (disk.get_type(new_path.disk_path()) != FileT::FILE)
				throw CommandException("(PickItemOption) " + itemfile + " is not a file");

			std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
			auto& maindisk = m_core.memory().get_disk(mainmark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.inventory")))
				maindisk.create(DiskPath("\\system", ".inventory"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);

			Command cmd("\"" + _command.get("path") + "\" move \"" + new_path.full_path_str() + "\" \"" + 
				mainmark + ":\\system\\.inventory\"");
			m_core.execute(cmd, User("hand of god", true, 255));

			return;
		}
	};
}