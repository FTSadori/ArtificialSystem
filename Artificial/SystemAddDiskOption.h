#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// adddisk {mark} {capacity} {max_sec_num} [::main]
	class SystemAddDiskOption final : public AbstractControllerOption
	{
	public:
		SystemAddDiskOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Adds disk info to DiskSystemInfo.\n");
				ptr->print_secondary("adddisk {mark} {capacity} {max_seg_num} [::main]\n");
				ptr->print_main("  mark - (string) new disk mark;\n");
				ptr->print_main("  capacity - (integer) disk capacity in bytes;\n");
				ptr->print_main("  max_sec_num - (integer) max number of sectors files;\n");
				ptr->print_main("  ::main - (flag) used for main disk partition;\n");

				return;
			}

			if (sender.lvl() < 255)
				throw PermissionException("(SystemAddDiskOption) Sender has low permission lvl");

			m_core.memory_info().disks_info.emplace_back(_command.get("1"),
				Memory::DiskInfo(
					_command.get("1"),
					Parser::from_string<size_t>(_command.get("2")),
					Parser::from_string<size_t>(_command.get("3"))
				)
			);

			if (_command.has("::main"))
				m_core.memory_info().main_disk = m_core.memory_info().disks_info.size() - 1;
		}
	};
}
