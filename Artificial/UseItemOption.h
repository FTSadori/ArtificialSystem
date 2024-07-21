#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Separator.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include "ItemUsingHandler.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// useitem
	class UseItemOption final : public AbstractControllerOption
	{
	public:
		UseItemOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("2 permission lvl needed\n");
				ptr->print_main("Uses item from Nova's inventory\n");
				ptr->print_main("Nova must be in current directory\n");
				ptr->print_secondary("useitem {name}\n");
				ptr->print_main("  name - (string) name of the item from inventory (without '[' and ']');\n");
				return;
			}

			using namespace Memory;

			if (sender.lvl() < 2)
				throw PermissionException("(UseItemOption) Sender has low permission lvl");

			FullPath nova_path = RelativePathCreator::combine(_command.get("path"), "nova.girl");
			auto& disk = m_core.memory().get_disk(nova_path.mark());
			if (!disk.is_exists(nova_path.disk_path()))
				throw CommandException("(UseItemOption) Nova is not here");

			std::string mainmark = m_core.memory_info().get_main_disk_info().mark;
			auto& maindisk = m_core.memory().get_disk(mainmark);
			if (!maindisk.is_exists(DiskPath("\\system")))
				maindisk.create(DiskPath("\\system"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);
			if (!maindisk.is_exists(DiskPath("\\system\\.inventory")))
				maindisk.create(DiskPath("\\system", ".inventory"), Permissions(true, 255, 255, 255, 0), FileT::DIR, true);

			auto list = maindisk.list(DiskPath("\\system\\.inventory"), sender.sudo());

			for (size_t i = 0; i < list.size(); ++i)
			{
				auto path = Memory::DiskPath("\\system\\.inventory", list[i]);

				DataQueue data = maindisk.read(path, true);
				std::string strdata(data.get_data(), data.size());
				
				auto splitted = Separator::split(strdata, '\n');
				if (splitted.size() == 0) continue;
				if (splitted[0].substr(1, splitted[0].size() - 2) == _command.get("1"))
				{
					ptr->print_main("She used item...\n");
					std::string script = Story::ItemUsingHandler::get_script_for_event(ptr->get_path(), _command.get("1"));
					if (script == "")
						ptr->print_main("...but nothing happened\n");
					else
					{
						Command cmd("\"" + ptr->get_path().full_path_str() + "\" run \"" + script + "\"");
						ptr->print_main("...and something happened\n");
						m_core.execute(cmd, User("amogus", true, 255));
						maindisk.remove(path, true);
					}
					return;
				}
			}

			ptr->print_main("She doesn't have this item\n");

			return;
		}
	};
}