#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// diskadd {mark} {capacity} {max_sec_num} [::main]
	class SystemDiskAddOption final : public AbstractControllerOption
	{
	public:
		SystemDiskAddOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("diskadd {mark} {capacity} {max_seg_num} [::main]");
				ptr->print_main("  mark - (string) new disk mark;");
				ptr->print_main("  capacity - (integer) disk capacity in bytes;");
				ptr->print_main("  max_sec_num - (integer) max number of sectors files;");
				ptr->print_main("  ::main - (flag) used for main disk partition;");

				return;
			}

			
		}
	};
}