#pragma once
#include "AbstractControllerOption.h"
#include "RelativePathCreator.h"
#include "CommandExceptions.h"
#include "Parser.h"
#include "Separator.h"
#include "Command.h"
#include "ICore.h"
#include "RealFileManager.h"
#include <map>
#include <filesystem>

namespace Commands
{
	// inv
	class ShowInventoryOption final : public AbstractControllerOption
	{
	public:
		ShowInventoryOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("2 permission lvl needed\n");
				ptr->print_main("Shows files in Nova's inventory\n");
				return;
			}

			using namespace Memory;

			if (sender.lvl() < 2)
				throw PermissionException("(ShowInventoryOption) Sender has low permission lvl");

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
				
				ptr->print_main(Parser::to_string(i) + ". ");
				
				DataQueue data = maindisk.read(path, true);
				std::string strdata(data.get_data(), data.size());
				try
				{
					auto splitted = Separator::split(strdata, '\n');
					ptr->print_main(splitted[0] + '\n');
					ptr->print_third(splitted[2]);
				}
				catch (...)
				{
					ptr->print_secondary("BAD FILE");
				}
				
				ptr->print_main("\n");
			}

			return;
		}
	};
}