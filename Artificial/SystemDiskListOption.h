#pragma once
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// disklist
	class SystemDiskListOption final : public AbstractControllerOption
	{
	public:
		SystemDiskListOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Shows info about all disk partitions in DiskSystemInfo.\n");
				
				return;
			}

			if (sender.lvl() < 255)
				throw PermissionException("(SystemDiskListOption) Sender has low permission lvl");
			
			if (m_core.memory_info().disks_info.empty())
				ptr->print_main("DiskSystemInfo is empty now\n");
			else
			{
				size_t i = 0;
				for (const auto& [mark, info] : m_core.memory_info().disks_info)
				{
					ptr->print_main(Parser::to_string(i) + ". ");
					ptr->print_secondary(mark + " ");
					ptr->print_main(Parser::to_string(info.capacity) + " bytes ");
					ptr->print_main("(" + Parser::to_string(info.max_sec_num) + " max sectors)");
					if (i++ == m_core.memory_info().main_disk)
						ptr->print_secondary(" (main)");
					ptr->print_secondary("\n");
				}
			}
		}
	};
}
