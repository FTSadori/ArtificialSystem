#pragma once
#include "AbstractControllerOption.h"
#include "CommandExceptions.h"
#include "Core.h"

namespace Commands
{
	// diskclear
	class SystemDiskClearOption final : public AbstractControllerOption
	{
	public:
		SystemDiskClearOption(ICore& _core)
			: AbstractControllerOption(_core) {}

		virtual void execute(const ICommand& _command, const User& sender) override
		{
			auto ptr = m_core.gui().get_terminal_ptr();

			if (_command.has("::help"))
			{
				ptr->print_main("255 permission lvl needed\n");
				ptr->print_main("Clears DiskSystemInfo.\n");
				
				return;
			}

			if (sender.lvl() < 255)
				throw PermissionException("(SystemDiskClearOption) Sender has low permission lvl");

			m_core.memory_info().disks_info.clear();
			ptr->print_main("DiskSystemInfo is now empty.\n");
		}
	};
}
